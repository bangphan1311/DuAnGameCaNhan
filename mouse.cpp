#include "Mouse.h"
#include <SDL_log.h>
#include "defs.h"

Mouse::Mouse(SDL_Texture* tex, int fw, int fh, int total, int startX, int startY, Mix_Chunk* jumpSfx)
    : x(startX), y(GROUND_Y - 45),
      frameWidth(fw), frameHeight(fh),
      totalFrames(total), currentFrame(0),
      frameDelay(5), frameTimer(0),
      dy(0), isJumping(false),
      texture(tex), jumpSound(jumpSfx),startY(startY) {}
      void Mouse::init() {
    y = GROUND_Y-45;  // Đảm bảo chuột bắt đầu từ mặt đất của chuột
    dy = 0;              // Đảm bảo vận tốc ban đầu là 0
    isJumping = false;   // Chuột không nhảy ngay từ đầu
}

void Mouse::jump() {
    if (!isJumping) {
        dy = JUMP_STRENGTH;
        isJumping = true;
        if (jumpSound) Mix_PlayChannel(-1, jumpSound, 0);
        else SDL_Log("Jump sound is NULL!");
    }
}

void Mouse::applyGravity() {
    if (isJumping) {
        dy += GRAVITY;
        y += dy;
        if (y >= GROUND_Y-45) {
            y = GROUND_Y-45;
            dy = 0;
            isJumping = false;
        }
    }
}

void Mouse::update() {
    applyGravity();
    if (!isJumping) {
        frameTimer++;
        if (frameTimer >= frameDelay) {
            currentFrame = (currentFrame + 1) % totalFrames;
            frameTimer = 0;
        }
    }
}
