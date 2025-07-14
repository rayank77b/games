// Simple Tower Defense Game in C++ with SDL2
// Make sure to link with SDL2: -lSDL2 -lSDL2main

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int ENEMY_SIZE = 20;
const int TOWER_SIZE = 40;
const int BULLET_SIZE = 5;
const int ENEMY_HEALTH = 3;

struct Bullet {
    float x, y;
    float dx, dy;
    float speed = 5.0f;

    void update() {
        x += dx * speed;
        y += dy * speed;
    }

    void draw(SDL_Renderer* renderer) {
        SDL_Rect rect = {(int)x, (int)y, BULLET_SIZE, BULLET_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
};

struct Enemy {
    int x, y;
    int speed;
    int health = ENEMY_HEALTH;

    void update() {
        x += speed;
    }

    void draw(SDL_Renderer* renderer) {
        SDL_Rect rect = {x, y, ENEMY_SIZE, ENEMY_SIZE};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    bool isHit(int bx, int by) {
        return bx > x && bx < x + ENEMY_SIZE && by > y && by < y + ENEMY_SIZE;
    }
};

struct Tower {
    int x, y;
    Uint32 lastShotTime = 0;

    void draw(SDL_Renderer* renderer) {
        SDL_Rect rect = {x, y, TOWER_SIZE, TOWER_SIZE};
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    bool canShoot() {
        return SDL_GetTicks() - lastShotTime > 1000;
    }

    Bullet shootAt(const Enemy& enemy) {
        float tx = x + TOWER_SIZE / 2.0f;
        float ty = y + TOWER_SIZE / 2.0f;
        float ex = enemy.x + ENEMY_SIZE / 2.0f;
        float ey = enemy.y + ENEMY_SIZE / 2.0f;
        float dx = ex - tx;
        float dy = ey - ty;
        float length = sqrt(dx * dx + dy * dy);
        dx /= length;
        dy /= length;
        lastShotTime = SDL_GetTicks();
        return Bullet{tx, ty, dx, dy};
    }
};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simple Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    std::vector<Bullet> bullets;

    Uint32 lastSpawnTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                towers.push_back({mx - TOWER_SIZE/2, my - TOWER_SIZE/2});
            }
        }

        if (SDL_GetTicks() - lastSpawnTime > 2000) {
            enemies.push_back({0, rand() % (SCREEN_HEIGHT - ENEMY_SIZE), 2});
            lastSpawnTime = SDL_GetTicks();
        }

        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].update();
        }

        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].update();
        }

        for (size_t i = 0; i < towers.size(); ++i) {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (towers[i].canShoot()) {
                    bullets.push_back(towers[i].shootAt(enemies[j]));
                    break;
                }
            }
        }

        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (enemies[j].isHit((int)bullets[i].x, (int)bullets[i].y)) {
                    enemies[j].health--;
                    bullets[i].x = -100; // move bullet out of screen
                    break;
                }
            }
        }

        // Remove dead enemies or enemies that exited screen
        std::vector<Enemy> newEnemies;
        for (size_t i = 0; i < enemies.size(); ++i) {
            if (enemies[i].health > 0 && enemies[i].x <= SCREEN_WIDTH) {
                newEnemies.push_back(enemies[i]);
            }
        }
        enemies = newEnemies;

        // Remove out-of-bound bullets
        std::vector<Bullet> newBullets;
        for (size_t i = 0; i < bullets.size(); ++i) {
            if (bullets[i].x >= 0 && bullets[i].x <= SCREEN_WIDTH && bullets[i].y >= 0 && bullets[i].y <= SCREEN_HEIGHT) {
                newBullets.push_back(bullets[i]);
            }
        }
        bullets = newBullets;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].draw(renderer);
        }

        for (size_t i = 0; i < towers.size(); ++i) {
            towers[i].draw(renderer);
        }

        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].draw(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
