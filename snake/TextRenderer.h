#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    bool init(const std::string& fontPath, int fontSize);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    void clean();
private:
    TTF_Font* font;
}; 