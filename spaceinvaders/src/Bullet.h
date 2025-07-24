#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(float x, float y, float dx, float dy, int speed);
    void update();
    void draw(SDL_Renderer* rdr) const;
    bool isOffScreen(int w,int h) const;
    float getX() const;
    float getY() const;
    void deactivate();
    
    static constexpr int W=5, H=15;

private:
    float x_, y_, dx_, dy_;
    int   speed_;
    
};
