#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_rect.h> 

#include <algorithm>
#include <cstdlib>
#include <string>
#include <fstream> 

#include "Game.h"
#include "Config.h"

Game::Game()
 : window_(nullptr), renderer_(nullptr),
   font_(nullptr), fontBig_(nullptr), running_(false), 
   lastSpawnTime_(0), lastMoneyTime_(0),
   score_(0), playMoney_(20)
{}

Game::~Game() {
    cleanUp();
}

bool Game::init(const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    if (TTF_Init() < 0) return false;                          // SDL_ttf
    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    if (!font_) return false;
    fontBig_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 30);
    if (!fontBig_) return false;

    window_ = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window_) return false;
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) return false;

    lastSpawnTime_ = SDL_GetTicks();
    spawnInterval_ = ENEMY_SPAWN_INTERVAL;
    running_       = true;
    paused_        = false;
    gameover_      = false;
    std::srand(static_cast<unsigned>(lastSpawnTime_));
    
    // load max score
    loadScore();

    return true;
}

void Game::run() {
    while (running_) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::cleanUp() {
    if (font_)     TTF_CloseFont(font_);
    if (fontBig_)     TTF_CloseFont(fontBig_);
    TTF_Quit(); 
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_)   SDL_DestroyWindow(window_);
    SDL_Quit();

    scoreMax_ = score_;
    saveScore();
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running_ = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    if(!gameover_) paused_ = !paused_;
                    break;
                default:
                    break;
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            SDL_GetMouseState(&mx, &my);

            if(playMoney_>=COST_PER_BUILD){
                // gewünschte Turm-Position
                int tx = mx - TOWER_SIZE/2;
                int ty = my - TOWER_SIZE/2;
                SDL_Rect newTowerRect{ tx, ty, TOWER_SIZE, TOWER_SIZE };

                // prüfen, ob's überschneidet
                bool overlap = false;
                for (auto& tower : towers_) {
                    SDL_Rect existing{ tower.getX(), tower.getY(), TOWER_SIZE, TOWER_SIZE };
                    if (SDL_HasIntersection(&newTowerRect, &existing)) {
                        overlap = true;
                        if(playMoney_>=COST_PER_UPGRADE) { // prueba upgrade
                            tower.setHealth(TOWER_HEALTH);
                            playMoney_ -= COST_PER_UPGRADE;
                        }
                        break;
                    }
                }
                // nur erstellen, wenn keine Kollision
                if (!overlap) {
                    towers_.emplace_back(tx, ty);
                    playMoney_ -= COST_PER_BUILD;
                    
                }
            }
        }
    }
}

void Game::spawnEnemy() {
    int y = std::rand() % (SCREEN_HEIGHT - ENEMY_SIZE);
    enemies_.emplace_back(0, y, 2);
}

void Game::update() {

    if(score_<SCORE_GAME_OVER) gameover_ = true;
    if(gameover_) return;
    if (paused_) return; 

    Uint32 now = SDL_GetTicks();
    if (now - lastSpawnTime_ > spawnInterval_) {
        spawnEnemy();
        lastSpawnTime_ = now;
        spawnInterval_ -= ENEMY_DECREASE_INTERVAL;  // immer schneller 
        if(spawnInterval_<ENEMY_MIN_INTERVAL)  spawnInterval_ = ENEMY_MIN_INTERVAL;
    }
    if (now - lastMoneyTime_ > TIME_TO_MONEY) {
        playMoney_++;  // erhoehe jede Sekunde
        lastMoneyTime_ = now;
    }

    // Bewegung
    for (auto& e : enemies_)  e.update();
    for (auto& b : bullets_)  b.update();
    for (auto& t : towers_)   t.update(enemies_, bullets_);

    // Treffer Kugel → Enemy
    for (auto& b : bullets_) {
        for (auto& e : enemies_) {
            if (e.isHit(b.getX(), b.getY())) {
                e.damage(1);
                b.deactivate();
                if (e.getHealth() <= 0) {
                    score_ += SCORE_PER_KILL;   // Punkt für Kill
                }
                break;
            }
        }
    }

    // Enemy erreicht rechten Rand → Escape
    for (auto& e : enemies_) {
        if (e.getX() > SCREEN_WIDTH) {
            //score_ += SCORE_PER_ESCAPE;
            gameover_ = true;
        }
    }

    // entferne aus dem Bild gefallene Kugeln
    bullets_.erase(
      std::remove_if(
        bullets_.begin(), bullets_.end(),
        [](const Bullet& b){
          return b.getX()<0 || b.getX()>SCREEN_WIDTH
              || b.getY()<0 || b.getY()>SCREEN_HEIGHT;
        }
      ),
      bullets_.end()
    );

    // entferne zerstörte oder entkommene Gegner
    enemies_.erase(
      std::remove_if(
        enemies_.begin(), enemies_.end(),
        [](const Enemy& e){
          return e.getHealth()<=0 || e.getX()>SCREEN_WIDTH;
        }
      ),
      enemies_.end()
    );

    // Enemy trifft Tower → Tower nimmt Schaden
    for (auto& e : enemies_) {
        for (auto& t : towers_) {
            // einfache AABB-Collision
            if (e.getX() + ENEMY_SIZE  > t.getX() &&
                e.getX()              < t.getX() + TOWER_SIZE &&
                e.getY() + ENEMY_SIZE > t.getY() &&
                e.getY()              < t.getY() + TOWER_SIZE)
            {
                t.damage(1);
                e.damage(ENEMY_HEALTH); // Enemy nach Kontakt “sterben”
                score_ += SCORE_PER_TOWER_HIT;
                break;
            }
        }
    }

    // Bullet vs. Tower Collision: deactivate bullet on hit
    for (auto& b : bullets_) {
        // Erstelle das Rechteck der Bullet
        SDL_Rect bulletRect{
            static_cast<int>(b.getX()),
            static_cast<int>(b.getY()),
            BULLET_SIZE,
            BULLET_SIZE
        };
        // TOFIX: BUG some bullets pass 
        for (auto& t : towers_) {
            if (t.getHealth() <= 0) continue;  // toter Turm -> überspringen
            if (t.getID() == b.getID() ) continue; // eigene Kugel ueberspringen
            SDL_Rect towerRect{ t.getX(), t.getY(), TOWER_SIZE, TOWER_SIZE };
            if (SDL_HasIntersection(&bulletRect, &towerRect)) {
                b.deactivate();  // verschiebe Bullet aus dem Bildschirm
                break;           // nicht mehrfach deaktivieren
            }
        }
    }

    // entferne zerstörte Türme
    towers_.erase(
      std::remove_if(towers_.begin(), towers_.end(),
        [](const Tower& t){
          return t.getHealth() <= 0;
        }),
      towers_.end()
    );
}

void Game::writeText(std::string& txt, SDL_Color& color,const int x,const int y) {
    SDL_Surface* surf = TTF_RenderText_Blended(font_, txt.c_str(), color);
    SDL_Texture* tex  = SDL_CreateTextureFromSurface(renderer_, surf);
    SDL_Rect dst{x, y, surf->w, surf->h};
    SDL_FreeSurface(surf);
    SDL_RenderCopy(renderer_, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void Game::renderStats() {
    // Score oben links
    SDL_Color black = {0,0,0,255};
    std::string txt = "Score: " + std::to_string(score_) + " Money: " + std::to_string(playMoney_);
    writeText(txt, black, 10, 10);

    if (paused_) {
        // Render "PAUSED" at center
        SDL_Color yellow = {255, 255, 0, 255};
        std::string txt = "PAUSE";
        writeText(txt, yellow, SCREEN_WIDTH/2-60, SCREEN_HEIGHT/2);
    }
    if (gameover_) {
        // Render "Gameover" at center
        SDL_Color red = {255, 0, 0, 255};
        std::string txt = "Game Over  Score: " + std::to_string(score_);
        if(score_ < scoreMax_) {
            txt += " Fail last max Score was " + std::to_string(scoreMax_);
        } else {
            txt += " Hurra new max Score, last was " + std::to_string(scoreMax_);
        }
        writeText(txt, red, SCREEN_WIDTH/2-160, SCREEN_HEIGHT/2);
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer_, 255,255,255,255);
    SDL_RenderClear(renderer_);

    for (auto& e : enemies_)  e.draw(renderer_);
    for (auto& t : towers_)   t.draw(renderer_);
    for (auto& b : bullets_)  b.draw(renderer_);

    // Stats zuletzt, damit sie oben liegen
    renderStats();

    SDL_RenderPresent(renderer_);
}

bool Game::loadScore(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        scoreMax_ = 0;          // Datei nicht gefunden: Score 0
        return false;
    }
    in >> scoreMax_;            // liest eine ganze Zahl
    if (in.fail()) {
        scoreMax_ = 0;          // fehlerhafte Datei: Score 0
        return false;
    }
    return true;
}

bool Game::saveScore(const std::string& path) const {
    std::ofstream out(path, std::ofstream::trunc);
    if (!out.is_open()) {
        return false;        // konnte nicht öffnen
    }
    out << scoreMax_;           // schreibt den Score als Zahl
    return out.good();
}