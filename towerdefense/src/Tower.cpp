#include <cmath>
#include <iostream>
#include <limits>
#include <cmath>

#include "Tower.h"
#include "Config.h"

Tower::Tower(int x, int y) {
    x_ = x;
    y_ = y;
    health_ = TOWER_HEALTH;
    lastShotTime_ = 0;
    globalID++;
    id_ = globalID;
}

void Tower::update(const std::vector<Enemy>& enemies,
                   std::vector<Bullet>& bullets)
{
    if (health_ <= 0 || enemies.empty() || !canShoot()) return;

    // Mittelpunkt des Towers
    float tx = x_ + TOWER_SIZE/2.0f;
    float ty = y_ + TOWER_SIZE/2.0f;

    // Suche nach dem nächsten Enemy
    const Enemy* nearest = nullptr;
    float minDist2 = std::numeric_limits<float>::max();

    for (const auto& e : enemies) {
        // Mittelpunkt des Enemys
        float ex = e.getX() + ENEMY_SIZE/2.0f;
        float ey = e.getY() + ENEMY_SIZE/2.0f;
        float dx = ex - tx;
        float dy = ey - ty;
        float dist2 = dx*dx + dy*dy;

        if (dist2 < minDist2) {
            minDist2 = dist2;
            nearest  = &e;
        }
    }

    // Auf den nächsten Enemy schießen
    if (nearest) {
        bullets.push_back(shootAt(*nearest));
        // shootAt() setzt lastShotTime_ intern
    }
}

void Tower::draw(SDL_Renderer* renderer) const {
    if (health_ <= 0) return;  // unsichtbar bei tot

    SDL_Rect r{ x_, y_, TOWER_SIZE, TOWER_SIZE };

    // Farbe je nach Health
    if      (health_ >= 3) SDL_SetRenderDrawColor(renderer,   0,255,  0,255); // Grün
    else if (health_ == 2) SDL_SetRenderDrawColor(renderer,   0,  0,255,255); // Blau
    else if (health_ == 1) SDL_SetRenderDrawColor(renderer, 128,128,128,255); // Grau
    else                  SDL_SetRenderDrawColor(renderer,   0,  0,  0,  0); // transparent

    SDL_RenderFillRect(renderer, &r);

    if(health_>3) {  // draw a black quadrat for upgraded tower
        SDL_Rect r{ x_+TOWER_SIZE/4, y_+TOWER_SIZE/4, TOWER_SIZE/2, TOWER_SIZE/2 };
        SDL_SetRenderDrawColor(renderer,   0, 0, 0,255);
        SDL_RenderFillRect(renderer, &r);
    }
}

void Tower::damage(int amount) {
    health_ -= amount;
}

int Tower::getHealth() const { return health_; }
void Tower::setHealth(int h) { health_ += h; }

int Tower::getX() const { return x_; }
int Tower::getY() const { return y_; }

Uint32 Tower::getID() const { return id_; }


bool Tower::canShoot() const {
    if( health_ > TOWER_HEALTH )
        return SDL_GetTicks() - lastShotTime_ > TOWER_SHOT_COOLDOWN_UPGRADE;
    else
        return SDL_GetTicks() - lastShotTime_ > TOWER_SHOT_COOLDOWN;
}

Bullet Tower::shootAt(const Enemy& e) {

    float tx = x_ + TOWER_SIZE/2.0f;
    float ty = y_ + TOWER_SIZE/2.0f;

    float ex = e.getX() + ENEMY_SIZE/2.0f;
    float ey = e.getY() + ENEMY_SIZE/2.0f;

    float dx = ex - tx;
    float dy = ey - ty;
    float len = std::sqrt(dx*dx + dy*dy);
    dx /= len; dy /= len;
    lastShotTime_ = SDL_GetTicks();
    return Bullet(tx, ty, dx, dy, id_);
}