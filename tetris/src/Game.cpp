#include "Game.h"
#include "shapes/AllShapes.h"
#include <cstdlib>
#include <ctime>

Game::Game()
 : score_(0), level_(0), totalLines_(0),
   lastDropTime_(0), dropInterval_(800) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

Game::~Game() = default;

bool Game::init(const char* title, int width, int height) {
    if (!renderer_.init(title, width, height)) return false;
    board_.reset();
    spawnTetromino();
    lastDropTime_ = SDL_GetTicks();
    return true;
}

void Game::spawnTetromino() {
    if (!next_) next_ = generateRandomTetromino();
    current_ = std::move(next_);
    current_->setPosition(Board::WIDTH/2, 0);
    next_    = generateRandomTetromino();
    // Game Over?
    if (!board_.canPlace(*current_)) {
        // einfach Beenden
        exit(0);
    }
}

std::unique_ptr<Tetromino> Game::generateRandomTetromino() {
    switch (std::rand() % 7) {
        case 0: return std::make_unique<IShape>();
        case 1: return std::make_unique<JShape>();
        case 2: return std::make_unique<LShape>();
        case 3: return std::make_unique<OShape>();
        case 4: return std::make_unique<SShape>();
        case 5: return std::make_unique<TShape>();
        default:return std::make_unique<ZShape>();
    }
}

void Game::lockTetromino() {
    board_.place(*current_);
    int cleared = board_.clearLines();
    if (cleared > 0) {
        totalLines_ += cleared;
        score_ += cleared * 100;
        level_ = totalLines_ / 10;
        dropInterval_ = std::max<Uint32>(100, 800 - level_ * 50);
    }
    spawnTetromino();
}

void Game::updateTiming() {
    Uint32 now = SDL_GetTicks();
    if (now - lastDropTime_ >= dropInterval_) {
        if (!board_.moveIfPossible(*current_, 0, 1)) {
            lockTetromino();
        }
        lastDropTime_ = now;
    }
}

void Game::run() {
    bool quit = false;
    while (!quit) {
        input_.handleInput(quit, *current_, board_);
        updateTiming();
        renderer_.clear();
        renderer_.drawBoard(board_);
        renderer_.drawTetromino(*current_);
        renderer_.drawNext(next_);
        renderer_.drawInfo(score_, level_, totalLines_);
        renderer_.present();
        SDL_Delay(16);
    }
}

void Game::cleanUp() {
    renderer_.cleanUp();
}