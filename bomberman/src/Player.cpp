#include "Player.h"
#include "Config.h"
#include <SDL3/SDL.h>

Player::Player(int gridX, int gridY,
               float speed, int maxBombs, float tileSize)
 : gridX_(gridX)
 , gridY_(gridY)
 , x_(gridX*tileSize)
 , y_(gridY*tileSize)
 , speed_(speed)
 , maxBombs_(maxBombs)
 , activeBombs_(0)
 , tileSize_(tileSize)
 , moveX_(0)
 , moveY_(0)
{}

void Player::handleInput(const SDL_Event& e) {
    if (e.type==SDL_EVENT_KEY_DOWN) {
        switch(e.key.key) {
        case SDLK_LEFT:  moveX_ = -1; break;
        case SDLK_RIGHT: moveX_ = +1; break;
        case SDLK_UP:    moveY_ = -1; break;
        case SDLK_DOWN:  moveY_ = +1; break;
        default: break;
        }
    } else if (e.type==SDL_EVENT_KEY_UP) {
        switch(e.key.key) {
        case SDLK_LEFT:  if(moveX_<0) moveX_=0; break;
        case SDLK_RIGHT: if(moveX_>0) moveX_=0; break;
        case SDLK_UP:    if(moveY_<0) moveY_=0; break;
        case SDLK_DOWN:  if(moveY_>0) moveY_=0; break;
        default: break;
        }
    }
}

void Player::update(float dt) {
    x_ += moveX_ * speed_ * dt;
    y_ += moveY_ * speed_ * dt;
    // Klemmen im Spielfeld, Raster‑Position neu berechnen
    gridX_ = int(x_/tileSize_);  
    gridY_ = int(y_/tileSize_);
}

void Player::render(SDL_Renderer* r) {
    // Körper
    SDL_FRect b{ x_, y_, tileSize_, tileSize_ };
    SDL_SetRenderDrawColor(r, 0,0,255,255);
    SDL_RenderFillRect(r,&b);
    // Helmaufsatz
    SDL_FRect h{ x_+10, y_-5, tileSize_-20, 5 };
    SDL_SetRenderDrawColor(r, 200,200,255,255);
    SDL_RenderFillRect(r,&h);
}

SDL_Rect Player::getBounds() const {
    return SDL_Rect{ int(x_), int(y_), int(tileSize_), int(tileSize_) };
}

bool Player::canDropBomb() const {
    return activeBombs_ < maxBombs_;
}

SDL_Point Player::getGridPos() const {
    return SDL_Point{gridX_, gridY_};
}

void Player::onPowerUp(int type) {
    // z.B. type 0 = +Bomb, 1 = +Range, …
    if (type==0) maxBombs_++;
}

void Player::onBombDropped() {
    if (activeBombs_ < maxBombs_) {
        ++activeBombs_;
    }
}

void Player::onBombExploded() {
    if (activeBombs_ > 0) {
        --activeBombs_;
    }
}