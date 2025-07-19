#include "TextRenderer.h"
#include <iostream>

TextRenderer::TextRenderer() : font(nullptr) {}
TextRenderer::~TextRenderer() {}

bool TextRenderer::init(const std::string& fontPath, int fontSize) {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cerr << "Failed to load font: " << fontPath << " Error: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    int textW, textH;
    SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
    SDL_Rect dstRect = {x, y, textW, textH};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void TextRenderer::clean() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();
} 