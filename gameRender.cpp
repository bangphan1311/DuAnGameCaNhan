#include "GameRender.h"

void render(const Mouse& mouse, const Graphics& gfx) {
    SDL_Rect src = {mouse.currentFrame * mouse.frameWidth, 0, mouse.frameWidth, mouse.frameHeight};
    SDL_Rect dest = {
        mouse.x,
        mouse.y,
        static_cast<int>(mouse.frameWidth * 1.5),
        static_cast<int>(mouse.frameHeight * 1.5)
    };
    SDL_RenderCopy(gfx.renderer, mouse.texture, &src, &dest);
}

