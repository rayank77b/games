// === Snake.cpp ===
#include "Snake.h"
#include "Food.h"

void Snake::init(int gridWidth, int gridHeight) {
    gridW = gridWidth; 
    gridH = gridHeight;
    body = {{gridW/2, gridH/2}};
    dir = {1, 0};
}

void Snake::move() {
    Point newHead = { body[0].x + dir.x, body[0].y + dir.y };
    body.insert(body.begin(), newHead);
    body.pop_back();
}

void Snake::changeDirection(SDL_Keycode key) {
    if (key == SDLK_UP && dir.y != 1) dir = {0, -1};
    else if (key == SDLK_DOWN && dir.y != -1) dir = {0, 1};
    else if (key == SDLK_LEFT && dir.x != 1) dir = {-1, 0};
    else if (key == SDLK_RIGHT && dir.x != -1) dir = {1, 0};
}

bool Snake::checkCollision() const {
    const Point& head = body[0];
    // check collision with wall-border
    if (head.x < 0 || head.y < 0 || head.x >= gridW || head.y >= gridH) 
        return true;

    // check collision with snake itself
    for (size_t i = 1; i < body.size(); ++i) {
        if (body[i].x == head.x && body[i].y == head.y) 
            return true;
    }
    // no collision ;)
    return false;
}

bool Snake::checkCollision(const  Food& food) const {
    // check collision of food with snake 
    for (size_t i = 0; i < body.size(); ++i) {
        if (body[i].x == food.getPosition().x && body[i].y == food.getPosition().y) 
            return true;
    }
    // no collision ;)
    return false;
}


bool Snake::eats(const Food& food) {
    if (body[0].x == food.getPosition().x && body[0].y == food.getPosition().y) {
        body.push_back(body.back());
        return true;
    }
    return false;
}

void Snake::render(SDL_Renderer* renderer, int gridSize) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& p : body) {
        SDL_Rect rect = { p.x * gridSize, p.y * gridSize, gridSize, gridSize };
        SDL_RenderFillRect(renderer, &rect);
    }
}