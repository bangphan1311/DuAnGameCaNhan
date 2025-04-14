#include "Obstacle.h"
#include "Mouse.h"

Obstacle::Obstacle(SDL_Texture* tex, int startX, int startY, int w, int h)
    : texture(tex), x(startX), y(startY), width(w), height(h) {}

void Obstacle::update(float speed) {
    x -= static_cast<int>(speed);
    if (x + width < 0) {
        x = SCREEN_WIDTH;
    }
}

void Obstacle::render(const Graphics& graphics) const {
    SDL_Rect dest = {x, y, width, height};
    SDL_RenderCopy(graphics.renderer, texture, NULL, &dest);
}

bool checkCollision(const Mouse& mouse, const Obstacle& obstacle) {
    SDL_Rect mouseRect = {mouse.x + 20, mouse.y + 30, 40, 40};
    SDL_Rect obstacleRect = {obstacle.x + 10, obstacle.y + 10, obstacle.width - 20, obstacle.height - 20};
    return SDL_HasIntersection(&mouseRect, &obstacleRect);
}
