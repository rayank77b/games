// === Game.cpp ===
#include "Game.h"
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false), gridSize(20) {}
Game::~Game() {}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        return false;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    screen_width = width;
    screen_height = height;

    running = (window && renderer);
    snake.init(width / gridSize, height / gridSize);
    food.spawn(width / gridSize, height / gridSize);

    if (!textRenderer.init("Arial.ttf", 24))
        return false;
    score = 0;

    paused = true;

    return running;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(100);
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
                    snake.changeDirection(e.key.keysym.sym);
                    break;
            }
        }
    }
}

void Game::update() {

    if (paused) return; 

    snake.move();
    if (snake.checkCollision()) {
        // Restart the game state
        snake.init(screen_width / gridSize, screen_height / gridSize);
        food.spawn(screen_width / gridSize, screen_height / gridSize);
        score = 0;
        paused = true;
        return;
    }
    if (snake.eats(food)) {
        do {
            food.spawn(screen_width / gridSize, screen_height / gridSize);
        } while(snake.checkCollision(food));
        score++;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    snake.render(renderer, gridSize);
    food.render(renderer, gridSize);
    
    // Score anzeigen
    SDL_Color white = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score);
    // Render score at top right
    textRenderer.renderText(renderer, scoreText, screen_width - 150, 10, white);

    if (paused) {
        SDL_Color yellow = {255, 255, 0, 255};
        // Render "PAUSED" at center
        textRenderer.renderText(renderer, "PAUSED", screen_width/2 - 60, screen_height/2 - 20, yellow);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    std::cout<<"max score: "<<score<<std::endl;
    textRenderer.clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

