#include "Enemy.h"
#include "Config.h"

Enemy::Enemy(int x, int y, int speed)
 : x_(x), y_(y), speed_(speed), health_(ENEMY_HEALTH) {}

void Enemy::update() {
    x_ += speed_;
}

void Enemy::draw(SDL_Renderer* renderer) const {
    // Rechteck definieren
    SDL_Rect r{ x_, y_, ENEMY_SIZE, ENEMY_SIZE };

    // Farbe je nach health setzen
    switch (health_) {
        case 3:
            SDL_SetRenderDrawColor(renderer, 255,   0,   0, 255); // Rot
            break;
        case 2:
            SDL_SetRenderDrawColor(renderer, 255, 165,   0, 255); // Orange
            break;
        case 1:
            SDL_SetRenderDrawColor(renderer, 255, 255,   0, 255); // Gelb
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Grau für alle anderen Fälle
            break;
    }

    // Quadrat füllen
    SDL_RenderFillRect(renderer, &r);
}

bool Enemy::isHit(float bx, float by) const {
    return bx > x_ && bx < x_+ENEMY_SIZE
        && by > y_ && by < y_+ENEMY_SIZE;
}

void Enemy::damage(int amount) {
    health_ -= amount;
}
