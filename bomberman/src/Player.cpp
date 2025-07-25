#include "Player.h"
#include "Config.h"
#include <SDL3/SDL.h>

Player::Player(int c,int r,int sp,int mb)
 : col_(c), row_(r), x_(c*40), y_(r*40),
   speed_(sp), maxBombs_(mb), activeBombs_(0),
   moveX_(0), moveY_(0), tileSize_(40)
{}

void Player::handleInput(const SDL_Event& e) {
    if (e.type==SDL_EVENT_KEY_DOWN) {
        switch(e.key.keysym.sym) {
        case SDLK_LEFT:  moveX_ = -1; break;
        case SDLK_RIGHT: moveX_ = +1; break;
        case SDLK_UP:    moveY_ = -1; break;
        case SDLK_DOWN:  moveY_ = +1; break;
        default: break;
        }
    } else if (e.type==SDL_EVENT_KEY_UP) {
        switch(e.key.keysym.sym) {
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
    col_ = int(x_/tileSize_);
    row_ = int(y_/tileSize_);
}

void Player::render(SDL_Renderer* r) {
    // Körper
    SDL_Rect b{ int(x_), int(y_), int(tileSize_), int(tileSize_) };
    SDL_SetRenderDrawColor(r, 0,0,255,255);
    SDL_RenderFillRect(r,&b);
    // Helmaufsatz
    SDL_Rect h{ int(x_)+10, int(y_)-5, int(tileSize_)-20, 5 };
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
    return SDL_Point{col_, row_};
}

void Player::onPowerUp(int type) {
    // z.B. type 0 = +Bomb, 1 = +Range, …
    if (type==0) maxBombs_++;
}
