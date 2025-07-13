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

    if (TTF_Init() < 0) 
        return false;
    font = TTF_OpenFont("FreeSans.ttf", 24); // oder eine andere TTF im Ordner
    if (!font) {
        std::cout<<"cant load FreeSans.ttf"<<std::endl;
        return false;
    }
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
    if (snake.checkCollision()) 
        running = false;
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
    SDL_Surface* surface = TTF_RenderText_Blended(font, scoreText.c_str(), white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textW, textH;
    SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
    SDL_Rect dstRect = {screen_width - textW - 10, 10, textW, textH};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    if (paused) {
        SDL_Color yellow = {255, 255, 0, 255};
        SDL_Surface* pauseSurf = TTF_RenderText_Blended(font, "PAUSED", yellow);
        SDL_Texture* pauseTex = SDL_CreateTextureFromSurface(renderer, pauseSurf);
        int tw, th;
        SDL_QueryTexture(pauseTex, nullptr, nullptr, &tw, &th);
        SDL_Rect dst = {screen_width/2 - tw / 2, screen_height/2 - th / 2, tw, th};
        SDL_RenderCopy(renderer, pauseTex, nullptr, &dst);
        SDL_FreeSurface(pauseSurf);
        SDL_DestroyTexture(pauseTex);
    }

    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::clean() {
    std::cout<<"max score: "<<score<<std::endl;
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

