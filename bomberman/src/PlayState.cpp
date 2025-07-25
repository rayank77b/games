#include "PlayState.h"
#include "Game.h"
#include <algorithm>
#include <cstdlib>

// Hilfsfunktion: tile→Pixel
static float tileToPixel(int t, float tileSize) {
    return t * tileSize;
}

PlayState::PlayState()
 : player_(0,0,0,0,0)   // wird in init deferred
 , map_()
 , lastEnemySpawn_(0)
 , initialized_(false)
{}

void PlayState::handleEvent(Game& game, const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
        switch (e.key.key) {
        case SDLK_P:
            game.changeState(Game::StateID::Menu);
            return;
        case SDLK_Q:
            game.requestQuit();
            return;
        case SDLK_SPACE:
            if (player_.canDropBomb()) {
                auto pos = player_.getGridPos();
                const auto& cfg = game.getConfig();
                bombs_.emplace_back(
                    pos.x, pos.y,
                    cfg.getInt("BombFuse",2000),
                    cfg.getInt("BombRange",2)
                );
                player_.onBombDropped();
            }
            return;
        }
    }
    // Spielerbewegung
    player_.handleInput(e);
}

void PlayState::update(Game& game, float delta) {
    // Lazy initialization once we have config and screen size
    if (!initialized_) {
        const auto& cfg = game.getConfig();
        int cols = cfg.getInt("GridCols",13);
        int rows = cfg.getInt("GridRows",11);
        float tileSize = float(game.w()) / cols;  // oder game.h()/rows
        map_.init(cols, rows, tileSize,
                  cfg.getFloat("SoftBlockDensity",0.6f));
        // Spieler in obere linke Ecke platzieren
        player_ = Player(1,1,
                         cfg.getInt("PlayerSpeed",150),
                         cfg.getInt("MaxBombs",1),
                         tileSize);
        // erste Gegner‑Welle
        lastEnemySpawn_ = SDL_GetTicks();
        initialized_ = true;
    }

    const auto& cfg = game.getConfig();
    Uint32 now = SDL_GetTicks();

    // Map‑ und Spieler‑Update
    map_.update(delta);
    player_.update(delta);

    // Bomben updaten
    for (auto& b : bombs_) {
        b.update(delta);
        if (b.shouldExplode(now)) {
            explosions_.push_back(b.explode()); 
            player_.onBombExploded();
        }
    }
    // Explosionen updaten
    for (auto& ex : explosions_) {
        ex.update(delta);
        // Kollision Explosion vs. weiche Blöcke
        for (auto& blk : map_.getSoftBlocks()) {
            if (!blk.destroyed && ex.intersects(blk.getBounds())) {
                blk.destroy();
                game.addScore(10);
                // ggf. Power‑up spawn via map_.spawnPowerUp(...)
            }
        }
        // Explosion vs. Spieler
        if (ex.intersects(player_.getBounds())) {
            game.changeState(Game::StateID::GameOver);
            return;
        }
        // Explosion vs. Gegner
        for (auto& en : enemies_) {
            if (!en.isDead() && ex.intersects(en.getBounds())) {
                en.kill();
                game.addScore(50);
            }
        }
    }

    // Gegner spawnen periodisch
    if (now - lastEnemySpawn_ >= Uint32(cfg.getInt("EnemyBombFuse",2000))) {
        // zufällig am Rand
        int c = std::rand() % map_.cols();
        enemies_.emplace_back(
            c, map_.rows()-2,
            cfg.getInt("EnemySpeed",100),
            cfg.getInt("EnemyMaxBombs",1),
            cfg.getInt("EnemyBombFuse",2000),
            cfg.getInt("EnemyBombRange",2),
            map_.getTileSize()
        );
        lastEnemySpawn_ = now;
    }

    // Gegner‑AI & Update
    for (auto& en : enemies_) {
        en.update(delta);
        // zufällig Bombe legen
        if (en.canDropBomb(now)) {
            auto pos = en.getGridPos();
            bombs_.emplace_back(
                pos.x, pos.y,
                cfg.getInt("EnemyBombFuse",2000),
                cfg.getInt("EnemyBombRange",2)
            );
            en.onBombDropped();
        }
        // Kollision Gegner vs. Spieler
        if (en.intersects(player_.getBounds())) {
            game.changeState(Game::StateID::GameOver);
            return;
        }
    }

    // Aufräumen: entferne explodierte Bomben
    bombs_.erase(
        std::remove_if(bombs_.begin(), bombs_.end(),
            [&](const Bomb& b){ return b.exploded(now); }),
        bombs_.end()
    );
    // entferne abgelaufene Explosionen
    explosions_.erase(
        std::remove_if(explosions_.begin(), explosions_.end(),
            [&](const Explosion& ex){ return ex.finished(); }),
        explosions_.end()
    );
    // entferne tote Gegner
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const Enemy& e){ return e.isDead(); }),
        enemies_.end()
    );
}

void PlayState::render(Game& game, SDL_Renderer* r) {
    // Map zeichnen (Boden, feste+weiche Blöcke, PowerUps)
    map_.render(r);

    // Bomben
    for (auto& b : bombs_) b.render(r);

    // Explosionen
    for (auto& ex : explosions_) ex.render(r);

    // Gegner
    for (auto& en : enemies_) en.render(r);

    // Spieler zuletzt
    player_.render(r);

    // HUD: Score
    SDL_Color white{255,255,255,255};
    auto txt = "Score: " + std::to_string(game.getScore());
    SDL_Surface* surf = TTF_RenderText_Blended(
        game.getFont(), txt.c_str(), 0, white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r, surf);
    SDL_FRect dstF{10.f, 10.f, float(surf->w), float(surf->h)};
    SDL_DestroySurface(surf);
    SDL_RenderTexture(r, tex, nullptr, &dstF);
    SDL_DestroyTexture(tex);
}
