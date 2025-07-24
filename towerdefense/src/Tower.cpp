#include "Tower.h"
#include "Config.h"
#include <cmath>
#include <iostream>

Tower::Tower(int x, int y) : x_(x), y_(y), health_(TOWER_HEALTH), lastShotTime_(0) {
        globalID++;
        id_ = globalID;
        std::cout << "Tower id: " << id_ << "\n";
}

void Tower::update(const std::vector<Enemy>& enemies,
                   std::vector<Bullet>& bullets)
{
    if (health_<=0 || enemies.empty() || !canShoot()) return;
    // Ziel: erster Enemy in der Liste
    // TOFIX: erster Enemy in der Nähe schiessen
    bullets.push_back(shootAt(enemies.front()));
}

void Tower::draw(SDL_Renderer* renderer) const {
    if (health_ <= 0) return;  // unsichtbar bei tot

    SDL_Rect r{ x_, y_, TOWER_SIZE, TOWER_SIZE };

    // Farbe je nach Health
    if      (health_ == 3) SDL_SetRenderDrawColor(renderer,   0,255,  0,255); // Grün
    else if (health_ == 2) SDL_SetRenderDrawColor(renderer,   0,  0,255,255); // Blau
    else if (health_ == 1) SDL_SetRenderDrawColor(renderer, 128,128,128,255); // Grau
    else                  SDL_SetRenderDrawColor(renderer,   0,  0,  0,  0); // transparent

    SDL_RenderFillRect(renderer, &r);
}

void Tower::damage(int amount) {
    health_ -= amount;
}

int Tower::getHealth() const { return health_; }

int Tower::getX() const { return x_; }
int Tower::getY() const { return y_; }

Uint32 Tower::getID() const { return id_; }


bool Tower::canShoot() const {
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