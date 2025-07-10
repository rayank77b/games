// === Game.cpp ===
#include "Game.h"
#include <iostream>
#include "Pos.h"

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}
Game::~Game() {}

bool Game::init(const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        return false;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ball.set(600, 600, 1, -2);
    bar.set(screen_width/2, 100, 20);

    for(int i=0; i<=screen_width-brick_width; i+=brick_width) {
        bricks.push_back(Brick(i, 100));
        bricks.push_back(Brick(i, 100+brick_height));
        bricks.push_back(Brick(i, 100+2*brick_height));
    }
    for(int i=0; i<=410; i+=brick_width) {  // die linke haelfte
        bricks.push_back(Brick(i, 100+3*brick_height));
        bricks.push_back(Brick(i, 100+4*brick_height));
        bricks.push_back(Brick(i, 100+5*brick_height));
    }

    running = (window && renderer);
    
    score = 0;

    paused = false;

    return running;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(5);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    paused = !paused;
                    break;
                default:
                    bar.changePosition(e.key.keysym.sym);
                    break;
            }
        }
    }
}

void Game::update() {
    if (paused) return; 

    ball.move();
    ball.checkCollisionBorder();
    ball.checkHitBar(bar);

    for( long unsigned int i=0; i<bricks.size(); i++ ) {
        if(ball.checkCollisionBrick(bricks[i])) {
            int bx = bricks[i].getPos();
            int by = bricks[i].getPosy();
            bricks.erase(bricks.begin()+i);
            ball.changeDirection(bx, by, brick_width, brick_height);
        }
    }

    if(ball.checkHitDownBorder()) {
        ball.set(200,300, 1,1);
    }
    
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    ball.render(renderer);
    bar.render(renderer);

    for(auto b : bricks) {
        b.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    std::cout<<"max score: "<<score<<std::endl;
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

    