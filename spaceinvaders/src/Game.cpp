#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <sstream>

#include "Game.h"

Game::Game()
 : win_(nullptr), ren_(nullptr), font_(nullptr),
   screenW_(0), screenH_(0),
   state_(State::Playing),
   player_(nullptr), lastSpawn_(0),
   baseECount_(0), baseESpeed_(0), baseEBInterval_(0), eSpawnInt_(0),
   level_(1), score_(0), highScore_(0),
   lvlEIncr_(0), lvlSpeedIncr_(0), lvlEBIntDec_(0)
{}

Game::~Game() { cleanUp(); }

// lädt alle Werte aus config.ini in die Game‑Member
void Game::loadConfig() {
    // lädt key=value Paare aus der Datei
    cfg_.load("config.ini");

    // Bildschirm
    screenW_        = cfg_.getInt("ScreenWidth", 1200);
    screenH_        = cfg_.getInt("ScreenHeight",1000);

    // Basis‑Enemy‑Einstellungen
    baseECount_     = cfg_.getInt("BaseEnemyCount",     5);
    baseESpeed_     = cfg_.getInt("EnemySpeed",        5);
    baseEBInterval_ = cfg_.getInt("EnemyBulletInterval",20);
    eSpawnInt_      = cfg_.getInt("EnemySpawnInterval",2000);

    // Level‑Skalierung
    lvlEIncr_       = cfg_.getInt("LevelEnemyIncrement",        1);
    lvlSpeedIncr_   = cfg_.getInt("LevelSpeedIncrement",       3);
    lvlEBIntDec_    = cfg_.getInt("LevelBulletIntervalDecrease",100);
}

bool Game::init(const char* title) {
    loadConfig();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)  return false;
    if (TTF_Init() < 0)                return false;
    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    if (!font_)                       return false;

    win_ = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        screenW_, screenH_, SDL_WINDOW_SHOWN);
    if (!win_) return false;
    ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED);
    if (!ren_) return false;

    loadHighScore();
    reset();
    return true;
}

void Game::loadHighScore() {
    std::ifstream in("score.txt");
    if (!in.is_open() || !(in>>highScore_)) highScore_ = 0;
}

void Game::saveHighScore() {
    if (score_ > highScore_) {
        std::ofstream out("score.txt",std::ofstream::trunc);
        if (out.is_open()) out<<score_;
    }
}

void Game::reset() {
    // auf Anfangswert
    level_ = 1;
    score_ = 0;
    enemies_.clear();
    pBullets_.clear();
    eBullets_.clear();
    lastSpawn_ = SDL_GetTicks();
    delete player_;
    player_ = new Player(
       screenW_/2 - Player::W/2,
       screenH_ - Player::H - 20,
       cfg_.getInt("PlayerSpeed",30),
       cfg_.getInt("PlayerBulletSpeed",30),
       cfg_.getInt("PlayerBulletInterval",100)
    );
    spawnWave();
    state_ = State::Playing;
}

void Game::spawnWave() {
    int cnt = baseECount_ + (level_-1)*lvlEIncr_;
    int spd = baseESpeed_ + (level_-1)*lvlSpeedIncr_;
    Uint32 bi = baseEBInterval_ - (level_-1)*lvlEBIntDec_;
    for (int i=0;i<cnt;++i) {
        int x = rand()%(screenW_-Enemy::W);
        enemies_.emplace_back(x, 0, spd,
                              cfg_.getInt("EnemyBulletSpeed",20),
                              bi);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type==SDL_QUIT) {
            state_ = State::GameOver;
            saveHighScore();
            SDL_Quit();
            exit(0);
        }
        if (state_==State::Playing) {
            player_->handleEvent(e);
            if (e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_SPACE) {
                Uint32 now = SDL_GetTicks();
                if (player_->canShoot(now)) {
                    pBullets_.push_back(player_->shoot(now));
                }
            }
        } else {
            if (e.type==SDL_KEYDOWN) {
                if (e.key.keysym.sym==SDLK_r) reset();
                if (e.key.keysym.sym==SDLK_q) {
                    saveHighScore();
                    SDL_Quit();
                    exit(0);
                }
            }
        }
    }
}

void Game::update() {
    if (state_!=State::Playing) return;
    Uint32 now = SDL_GetTicks();

    // neue Welle?
    if (enemies_.empty()) {
        level_++;
        spawnWave();
    }

    // spawn nach Zeit?
    if (now - lastSpawn_ >= eSpawnInt_) {
        spawnWave();
        lastSpawn_ = now;
    }

    // Bewegungen
    player_->update(screenW_, screenH_);
    for(auto& b: pBullets_) b.update();
    for(auto& b: eBullets_) b.update();
    for(auto& en: enemies_) en.update();

    // 1) Treffer verarbeiten: nur deaktivieren
    for (auto& b : pBullets_) {
        for(auto it=enemies_.begin(); it!=enemies_.end(); ++it) {
            if (it->isHit(b)) {
                b.deactivate();   // setzt x_ = -100 o.Ä.
                enemies_.erase(it);
                score_++;
                break;
            }
        }
    }
    // 2) Ganz am Ende des update(): alle inaktiven / off‑screen Bullets löschen
    pBullets_.erase(
        std::remove_if(
            pBullets_.begin(), pBullets_.end(),
            [&](const Bullet& b){
                return b.getX() < 0 || b.getX() > screenW_
                    || b.getY() < 0 || b.getY() > screenH_;
            }
        ),
        pBullets_.end()
    );

    // Enemy schießt
    for(auto& en: enemies_) {
        if (en.readyToFire(now)) {
            eBullets_.push_back(en.fire(now));
        }
    }

    // test if enemy get the bottom, then game over
    for(auto& en: enemies_) {
        if(en.reachedBottom(screenH_)) {
            state_ = State::GameOver;
            saveHighScore();
        }
    }

    // Enemy‑Bullet vs Player
    for(auto& b: eBullets_) {
        if ( b.getX()>player_->getX() &&
             b.getX()<player_->getX()+Player::W &&
             b.getY()>player_->getY() &&
             b.getY()<player_->getY()+Player::H )
        {
            state_ = State::GameOver;
            saveHighScore();
        }
    }

    // Leerung der Off‑Screen‑Bullets und getöteter Bullets
    pBullets_.erase(
      std::remove_if(pBullets_.begin(),pBullets_.end(),
        [&](const Bullet& b){ return b.isOffScreen(screenW_,screenH_); }),
      pBullets_.end());
    eBullets_.erase(
      std::remove_if(eBullets_.begin(),eBullets_.end(),
        [&](const Bullet& b){ return b.isOffScreen(screenW_,screenH_); }),
      eBullets_.end());
}

void Game::render() {
    SDL_SetRenderDrawColor(ren_,  0,  0,  0,255);
    SDL_RenderClear(ren_);

    if (state_==State::Playing) {
        player_->draw(ren_);
        for(auto& en: enemies_) en.draw(ren_);
        for(auto& b: pBullets_) b.draw(ren_);
        for(auto& b: eBullets_) b.draw(ren_);
        // HUD: Score & Level
        std::stringstream ss;
        ss<<"Score: "<<score_<<"  Level: "<<level_;
        SDL_Surface* surf = TTF_RenderText_Blended(font_,ss.str().c_str(),{255,255,255,255});
        SDL_Texture* tex  = SDL_CreateTextureFromSurface(ren_,surf);
        SDL_Rect dst{10,10, surf->w, surf->h};
        SDL_FreeSurface(surf);
        SDL_RenderCopy(ren_,tex,nullptr,&dst);
        SDL_DestroyTexture(tex);
    }
    else {
        // Game Over Screen
        SDL_Surface* s1 = TTF_RenderText_Blended(font_,"--- GAME OVER ---",{255,0,0,255});
        SDL_Surface* s2 = TTF_RenderText_Blended(font_,
           (score_>highScore_?"!!! NEW HIGHSCORE !!!":"Press R to restart, Q to quit"),{255,255,0,255});
        SDL_Surface* s3 = TTF_RenderText_Blended(font_,("Your Score: "+std::to_string(score_)).c_str(),{255,255,255,255});
        SDL_Surface* s4 = TTF_RenderText_Blended(font_,("HighScore: "+std::to_string(highScore_)).c_str(),{255,255,255,255});
        SDL_Texture *t1=SDL_CreateTextureFromSurface(ren_,s1),
                    *t2=SDL_CreateTextureFromSurface(ren_,s2),
                    *t3=SDL_CreateTextureFromSurface(ren_,s3),
                    *t4=SDL_CreateTextureFromSurface(ren_,s4);
        int cx = screenW_/2;
        auto drawSurf=[&](SDL_Surface* surf, SDL_Texture* tex, int py){
            SDL_Rect dst{ cx-surf->w/2, py, surf->w, surf->h };
            SDL_RenderCopy(ren_,tex,nullptr,&dst);
        };
        drawSurf(s1,t1, screenH_/2-100);
        drawSurf(s2,t2, screenH_/2- 50);
        drawSurf(s3,t3, screenH_/2     );
        drawSurf(s4,t4, screenH_/2+ 50);
        SDL_FreeSurface(s1); SDL_FreeSurface(s2);
        SDL_FreeSurface(s3); SDL_FreeSurface(s4);
        SDL_DestroyTexture(t1);SDL_DestroyTexture(t2);
        SDL_DestroyTexture(t3);SDL_DestroyTexture(t4);
    }

    SDL_RenderPresent(ren_);
}

void Game::run() {
    while(true) {
        handleEvents();
        update();
        render();
        SDL_Delay(100);
    }
}

void Game::cleanUp() {
    delete player_;
    if (font_)     TTF_CloseFont(font_);
    TTF_Quit();
    if (ren_)      SDL_DestroyRenderer(ren_);
    if (win_)      SDL_DestroyWindow(win_);
    SDL_Quit();
}
