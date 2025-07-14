// Mondlander in SDL2 und C++ mit Punktanzeige
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <cmath>

// Fenstergrößen
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Lander-Parameter
struct Lander {
    float x = SCREEN_WIDTH / 2.0f;
    float y = 50.0f;
    float vy = 0.0f;  // vertikale Geschwindigkeit
    float fuel = 100.0f;
    bool thrusting = false;
};

// Konstanten
const float GRAVITY = 0.001f;
const float THRUST = -0.1f;
const float GROUND_Y = SCREEN_HEIGHT - 50;

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer, TTF_Font* font, const Lander& lander, bool gameOver) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Boden
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_Rect ground = { 0, (int)GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - (int)GROUND_Y };
    SDL_RenderFillRect(renderer, &ground);

    // Lander
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = { (int)lander.x - 10, (int)lander.y - 10, 20, 20 };
    SDL_RenderFillRect(renderer, &rect);

    // Flamme
    if (lander.thrusting && lander.fuel > 0) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);
        SDL_RenderDrawLine(renderer, (int)lander.x, (int)lander.y + 10,
                           (int)lander.x, (int)lander.y + 20);
    }

    // Textanzeige
    std::ostringstream ss;
    ss << "Treibstoff: " << (int)lander.fuel << "  Geschwindigkeit: " << lander.vy;
    renderText(renderer, font, ss.str(), 20, 20);

    if (gameOver) {
        std::string result = std::string("Landung: ") + (fabs(lander.vy) < 2.0f ? "Erfolgreich!" : "Abgestürzt!");
        renderText(renderer, font, result, 20, 50);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Fehler: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() != 0) {
        std::cerr << "TTF Init Fehler: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Mondlander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16);
    if (!font) {
        std::cerr << "Konnte Schriftart nicht laden." << std::endl;
        return 1;
    }

    Lander lander;
    bool running = true;
    bool gameOver = false;
    SDL_Event event;

    while (running) {
        lander.thrusting = false;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
            if (!gameOver && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && lander.fuel > 0) {
                lander.thrusting = true;
            }
        }

        if (!gameOver) {
            if (lander.thrusting && lander.fuel > 0) {
                lander.vy += THRUST;
                lander.fuel -= 0.2f;
            }

            lander.vy += GRAVITY;
            lander.y += lander.vy;

            if (lander.y >= GROUND_Y - 10) {
                lander.y = GROUND_Y - 10;
                gameOver = true;
                std::cout << "Landung: " << (fabs(lander.vy) < 2.0f ? "Erfolgreich!" : "Abgestürzt!") << std::endl;
            }
        }

        render(renderer, font, lander, gameOver);
        SDL_Delay(16); // ~60 FPS
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
