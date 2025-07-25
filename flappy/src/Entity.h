#pragma once
#include <SDL3/SDL.h>

// Basis‐Klasse für Zeichnen und Aktualisieren
class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float delta) = 0;
    virtual void render(SDL_Renderer* rdr) = 0;
    virtual SDL_FRect getBounds() const = 0;
};
