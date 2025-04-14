#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include "graphics.h"
#include "GameDefines.h"
struct Mouse; // Forward declaration

struct Obstacle {
    int x, y, width, height;
    SDL_Texture* texture;

    Obstacle(SDL_Texture* tex, int startX, int startY, int w, int h);

    void update(float speed);
    void render(const Graphics& graphics) const;
};

bool checkCollision(const Mouse& mouse, const Obstacle& obstacle);

#endif
