#include "Player.h"

Player::Player(int x,int y,int sp,int bs,Uint32 si)
 : x_(x), y_(y),
   speed_(sp),
   bSpeed_(bs),
   lastShot_(0),
   shootInterval_(si),
   movLeft_(false), movRight_(false)
{}

void Player::handleEvent(const SDL_Event& e) {
    if (e.type==SDL_KEYDOWN) {
        if      (e.key.keysym.sym==SDLK_LEFT)  movLeft_= true;
        else if (e.key.keysym.sym==SDLK_RIGHT) movRight_=true;
    }
    else if (e.type==SDL_KEYUP) {
        if      (e.key.keysym.sym==SDLK_LEFT)  movLeft_= false;
        else if (e.key.keysym.sym==SDLK_RIGHT) movRight_=false;
    }
}

void Player::update(int screenWidth, int screenHeight) {
    if (movLeft_)  x_ -= speed_;
    if (movRight_) x_ += speed_;

    // links und rechts klemmen
    if (x_ < 0)           x_ = 0;
    if (x_ + W > screenWidth)  x_ = screenWidth - W;

    // wenn du später auch vertikal klemmen willst:
    if (y_ < 0)           y_ = 0;
    if (y_ + H > screenHeight) y_ = screenHeight - H;
}

void Player::draw(SDL_Renderer* r) const {
    // Schiffkörper
    SDL_SetRenderDrawColor(r,0,200,0,255);
    SDL_Rect body{ x_, y_, W, H };
    SDL_RenderFillRect(r,&body);
    // Turm
    SDL_SetRenderDrawColor(r,0,150,0,255);
    SDL_Rect turret{ x_+W/3, y_-10, W/3, 10 };
    SDL_RenderFillRect(r,&turret);
}

bool Player::canShoot(Uint32 now) const {
    return (now - lastShot_) >= shootInterval_;
}

Bullet Player::shoot(Uint32 now) {
    lastShot_ = now;
    return Bullet(x_+W/2, y_,  0.0f, -1.0f, bSpeed_);
}

float Player::getX() const { return x_; }
float Player::getY() const { return y_; }
