#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "GameDefines.h"

struct Mouse {
    int x, y;
    int frameWidth, frameHeight;
    int totalFrames, currentFrame;
    int frameDelay, frameTimer;
    int startY;
    int dy;
    bool isJumping;
    SDL_Texture* texture;
    Mix_Chunk* jumpSound;

    Mouse(SDL_Texture* tex, int fw, int fh, int total, int startX, int startY, Mix_Chunk* jumpSfx);

    void init();
    void reset();
    void jump();
    void applyGravity();
    void update();
};

#endif
