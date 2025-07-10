// === Ball.cpp ===
#include "Ball.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

Ball::Ball() {
    pos.x = 0; pos.y = 0;
    velocity.vx = 0; velocity.vy = 0;
    radius = 20;
    debugtimer2 = 100;
}

void Ball::set(int x, int y, int vx, int vy, int r) {
    pos.x = x;
    pos.y = y;
    velocity.vx = vx;
    velocity.vy = vy;
    radius = r;
}

void Ball::setPos(const Pos p) {
    pos.x = p.x;
    pos.y = p.y;
}

void Ball::setVelocity(const Velocity v) {
    velocity.vx = v.vx;
    velocity.vy = v.vy;
}

void Ball::move(){
    pos.x += velocity.vx;
    pos.y += velocity.vy;
    /*debugtimer2--;
    if(debugtimer2 == 0) {
        std::cout<<pos.x<<" | "<<pos.y<<"  "<<velocity.vx<<" | "<<velocity.vy<<std::endl;
        debugtimer2 = 100;
    }*/
}

void Ball::checkCollisionBorder() {
    // test for Left Border
    if(pos.x <= 0) velocity.vx *= -1;
    // test for right Border
    if((pos.x+radius)>=screen_width) velocity.vx *= -1;
    // test for top border
    if(pos.y <= 0) velocity.vy *= -1;
}

bool Ball::checkHitDownBorder(){
    if(pos.y >= screen_height)  
        return true;
    return false;
}

void Ball::checkHitBar(const Bar bar) {
    int bx = bar.getPos();
    int by = bar.getPosy();
    int bw = bar.getWidth();
    int bh = bar.getHeight();

    if(checkCollision(bx, by, bw, bh)){
            // reflektiere nach oben (vy negativ)
            if(velocity.vy>0) velocity.vy *= -1;
            // set ball position, so not collide in bar
            pos.y = by - radius - 1;

            // berechne wo der Ball getroffen hat um vx geschwindigkeit zu aendern
            int l = pos.x+radius/2 - bx;
            float d = float(l)/float(bw);  
            //std::cout<<"x: "<<pos.x+radius/2<<"  bx: "<<bx<<std::endl;
            //std::cout<<"l,bw: "<<l<<" "<<bw<<" d: "<<d<<std::endl;
            //std::cout<<"vx_old: "<<velocity.vx;
            if( d<0.1 ) velocity.vx -= 2;
            if( d>=0.1 && d<0.3) velocity.vx--;
            if( d>0.7 && d<=0.9) velocity.vx++;
            if( d>0.9 ) velocity.vx += 2;
            //std::cout<<"   vx_new: "<<velocity.vx<<std::endl;
        
    } 
}

bool Ball::checkCollisionBrick(const Brick b)
{
    int bx = b.getPos();
    int by = b.getPosy();
    int bw = brick_width;
    int bh = brick_height;

    return checkCollision(bx, by, bw, bh);

}

bool Ball::checkCollision(const int bx, const int by, const int bw, const int bh)
{
    return !(pos.x + radius  < bx      ||
             pos.x           > bx + bw ||
             pos.y + radius  < by      ||
             pos.y           > by + bh );

}

void Ball::changeDirection(const int bx, const int by, const int bw, const int bh) {
    if(velocity.vy<0) { // es fliegt von unten
        if((pos.x+radius)>bx && (pos.x<(bx+bw))) { // trifft von unten
            if(pos.y<=(by+bh)) {
                velocity.vy *= -1;
                return ;
            }
        } 
    } else if(velocity.vy>0) { // es fliegt von oben
        if((pos.x+radius)>bx && (pos.x<(bx+bw))) { // trifft von oben
            if((pos.y+radius)>=by) {
                velocity.vy *= -1;
                return ;
            }
        } 
    }
    if(velocity.vx<0) { // es fliegt von recht
        if((pos.y+radius)>by && (pos.y<(by+bh))) { // trifft rechten rand
            if(pos.x<=(bx+bw)) {
                velocity.vx *= -1;
                return ;
            }
        } 
    } else if(velocity.vx>0) { // es fliegt von links
        if((pos.y+radius)>by && (pos.y<(by+bh))) { // trifft linken rand
            if((pos.x+radius)>=bx) {
                velocity.vx *= -1;
                return ;
            }
        } 
    }
}

void Ball::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { pos.x, pos.y, radius, radius };
    SDL_RenderFillRect(renderer, &rect);
}
