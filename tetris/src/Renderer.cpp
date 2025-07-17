#include "Renderer.h"
#include <string>

Renderer::Renderer()
 : window_(nullptr), renderer_(nullptr), font_(nullptr) {}

Renderer::~Renderer() = default;

bool Renderer::init(const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() < 0) return false;

    window_ = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, SDL_WINDOW_SHOWN);
    if (!window_) return false;

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) return false;

    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    if (!font_) return false;

    infoRect_ = { Board::WIDTH * BLOCK_SIZE, 0, INFO_WIDTH, SCREEN_HEIGHT };
    return true;
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer_, 0,0,0,255);
    SDL_RenderClear(renderer_);
}

void Renderer::drawBoard(const Board& b) {
    for (int y=0; y<Board::HEIGHT; ++y)
    for (int x=0; x<Board::WIDTH;  ++x) {
        SDL_Color c = b.cell(x,y);
        SDL_Rect r = { x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE-1, BLOCK_SIZE-1 };
        SDL_SetRenderDrawColor(renderer_, c.r,c.g,c.b,255);
        SDL_RenderFillRect(renderer_, &r);
    }
}

void Renderer::drawTetromino(const Tetromino& t) {
    SDL_Color c = t.getColor();
    for (auto b : t.getBlocks()) {
        int x = (t.getPosition().x + b.x) * BLOCK_SIZE;
        int y = (t.getPosition().y + b.y) * BLOCK_SIZE;
        SDL_Rect r = { x, y, BLOCK_SIZE-1, BLOCK_SIZE-1 };
        SDL_SetRenderDrawColor(renderer_, c.r,c.g,c.b,255);
        SDL_RenderFillRect(renderer_, &r);
    }
}

void Renderer::drawNext(const std::unique_ptr<Tetromino>& next) {
    // kleines Vorschau-Feld
    SDL_Rect bg = { infoRect_.x + 10, 10, BLOCK_SIZE*4, BLOCK_SIZE*4 };
    SDL_SetRenderDrawColor(renderer_, 50,50,50,255);
    SDL_RenderFillRect(renderer_, &bg);
    // zentriert rendern
    SDL_Color c = next->getColor();
    for (auto b : next->getBlocks()) {
        int x = bg.x + (b.x+1) * BLOCK_SIZE;
        int y = bg.y + (b.y+1) * BLOCK_SIZE;
        SDL_Rect r = { x, y, BLOCK_SIZE-1, BLOCK_SIZE-1 };
        SDL_SetRenderDrawColor(renderer_, c.r,c.g,c.b,255);
        SDL_RenderFillRect(renderer_, &r);
    }
}

void Renderer::drawInfo(int score, int level, int lines) {
    // Hintergrund
    SDL_SetRenderDrawColor(renderer_, 30,30,30,255);
    SDL_RenderFillRect(renderer_, &infoRect_);

    auto renderText = [&](int y, const std::string& text){
        SDL_Surface* surf = TTF_RenderText_Blended(font_, text.c_str(), {255,255,255,255});
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surf);
        SDL_Rect dst = { infoRect_.x + 10, y, surf->w, surf->h };
        SDL_FreeSurface(surf);
        SDL_RenderCopy(renderer_, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    };

    renderText(200, "Score: " + std::to_string(score));
    renderText(230, "Level: " + std::to_string(level));
    renderText(260, "Lines: " + std::to_string(lines));
}

void Renderer::present() {
    SDL_RenderPresent(renderer_);
}

void Renderer::cleanUp() {
    if (font_)     TTF_CloseFont(font_);
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_)   SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}
