#pragma once
#include <SDL2/SDL.h>

// Bildschirm
constexpr int SCREEN_WIDTH            = 800;
constexpr int SCREEN_HEIGHT           = 600;

// Größen
constexpr int ENEMY_SIZE              = 20;
constexpr int TOWER_SIZE              = 40;
constexpr int BULLET_SIZE             = 5;

// Gameplay
constexpr int ENEMY_HEALTH            = 3;
constexpr int TOWER_HEALTH            = 3;
constexpr Uint32 ENEMY_SPAWN_INTERVAL = 2000;  // ms
constexpr Uint32 ENEMY_DECREASE_INTERVAL = 10; // ms
constexpr Uint32 ENEMY_MIN_INTERVAL = 50; // ms

constexpr Uint32 TOWER_SHOT_COOLDOWN  = 1000;  // ms
constexpr Uint32 TOWER_SHOT_COOLDOWN_UPGRADE = 250; // ms, shot 4x faster
constexpr Uint32 TIME_TO_MONEY        = 1000;  // ms

// Scoring
constexpr int SCORE_PER_KILL          =  10;
constexpr int SCORE_PER_ESCAPE        = -5;
constexpr int SCORE_PER_TOWER_HIT     = -1;

constexpr int COST_PER_BUILD          = 2;
constexpr int COST_PER_UPGRADE        = 5;

constexpr int SCORE_GAME_OVER         = -10;