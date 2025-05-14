#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include "graphics.h"
#include "GameDefines.h"
struct Mouse;

struct Obstacle {
    int x, y, width, height;
    SDL_Texture* texture;
    int type;


    Obstacle(SDL_Texture* tex, int startX, int startY, int w, int h, int obstacleType);

    void update(float speed);
    void render(const Graphics& graphics) const;

    int getX() const { return x; }
    int getWidth() const { return width; }
};


bool checkCollision(const Mouse& mouse, const Obstacle& obstacle);

#endif
