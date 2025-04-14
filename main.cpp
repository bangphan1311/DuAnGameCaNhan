#include <SDL.h>
#include <vector>
#include "graphics.h"
#include "game.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

#include "Mouse.h"
#include "Obstacle.h"
#include "GameRender.h"
#include "GameDefines.h"

int main(int argc, char* argv[]) {
    Graphics gfx;
    gfx.init();
    if (TTF_Init() == -1) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
    }

    TTF_Font* font = TTF_OpenFont("ARIBL0.ttf", 40); // bạn cần một file font TTF
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return -1;
    }

    ///am thanh
    Mix_Music* bgm = Mix_LoadMUS("energetic-chiptune-video-game-music-platformer-8-bit-318348.mp3");
    if (!bgm) {
        SDL_Log("Failed to load background music! SDL_mixer Error: %s", Mix_GetError());
        return -1;
    }

    Mix_PlayMusic(bgm, -1);

    Mix_Music* gameOverMusic = Mix_LoadMUS("hieu_ung_am_thanh_mat_mang-www_tiengdong_com.mp3");
    if (!gameOverMusic) {
        SDL_Log("Failed to load game over music! SDL_mixer Error: %s", Mix_GetError());
        return -1;
    }

    Mix_Chunk* jumpSound = Mix_LoadWAV("hieu_ung_am_thanh_nhay_qua_buoc_qua-www_tiengdong_com (mp3cut.net).mp3");
    if (!jumpSound) {
        SDL_Log("Failed to load jump sound! SDL_mixer Error: %s", Mix_GetError());
        return -1;
    }

    ///chuong ngai vat
    SDL_Texture* obstacleTex1 = gfx.loadTexture("bt-removebg-preview.png");
    SDL_Texture* obstacleTex2 = gfx.loadTexture("rắn-removebg-preview.png");
    SDL_Texture* obstacleTex3 = gfx.loadTexture("gầ-removebg-preview.png");

    std::vector<Obstacle> obstacles;
    obstacles.push_back(Obstacle(obstacleTex1, 500, GROUND_Y + 5, 70, 70));
    obstacles.push_back(Obstacle(obstacleTex2, 1000, GROUND_Y - 10, 80, 85));
    obstacles.push_back(Obstacle(obstacleTex3, 2500, GROUND_Y - 8, 80, 80));

    SDL_Texture* mouseTexture = gfx.loadTexture("shin.png");
    Mouse mouse(mouseTexture, 67, 76, 6, 100, GROUND_Y-45, jumpSound);

    SDL_Texture* background = gfx.loadTexture("bground.jpg");
    SDL_Texture* gameOverTexture = gfx.loadTexture("ảnh (2).jpg");

    ///diem
    int score = 0;
    Uint32 lastScoreTime = SDL_GetTicks();

    SDL_Color textColor = {255, 255, 255};

    SDL_Texture* scoreTexture = nullptr;
    SDL_Rect scoreRect;

    bool running = true;
    bool gameOver = false;
    int bgX = 0;
    float currentSpeed = 5.0f;
    Uint32 lastSpeedIncrease = SDL_GetTicks();
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (!gameOver && event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                    case SDLK_UP:
                        mouse.jump();
                        break;
                }
            }
        }

        if (!gameOver) {
            mouse.update();
            for (auto& obstacle : obstacles) {
                obstacle.update(currentSpeed);
                if (checkCollision(mouse, obstacle)) {
                    SDL_Log("Game Over!");
                    gameOver = true;
                    Mix_HaltMusic();
                    Mix_PlayMusic(gameOverMusic, -1);
                }
            }

            ///tang toc do
            if (SDL_GetTicks() - lastSpeedIncrease > 10000) {
                currentSpeed += 0.5f;
                if (currentSpeed > 15.0f) currentSpeed = 15.0f;
                lastSpeedIncrease = SDL_GetTicks();
            }

            if (!gameOver) {
                bgX -= (int)currentSpeed;
                if (bgX <= -SCREEN_WIDTH) {
                    bgX = 0;
                }
            }
        }

        if (!gameOver && SDL_GetTicks() - lastScoreTime > 500) {
            score++;
            lastScoreTime = SDL_GetTicks();

           ///tao score
            std::string scoreText = "Score: " + std::to_string(score);
            SDL_Log("Score Text: %s", scoreText.c_str());

            SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
            if (textSurface) {
                if (scoreTexture) SDL_DestroyTexture(scoreTexture); // xoá texture cũ
                scoreTexture = SDL_CreateTextureFromSurface(gfx.renderer, textSurface);
                scoreRect = { SCREEN_WIDTH - textSurface->w - 20, 20, textSurface->w, textSurface->h };
                SDL_FreeSurface(textSurface);
            }
        }

        ///ve nen
        // Vẽ màn hình
gfx.prepareScene();
gfx.renderScrollingBackground(background, bgX);

if (gameOver) {

    SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(gfx.renderer, gameOverTexture, NULL, &dest);

    /// diem khi thua
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Log("Score Text: %s", scoreText.c_str());

    // Tạo surface rồi chuyển sang texture
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* gameOverScoreTexture = SDL_CreateTextureFromSurface(gfx.renderer, textSurface);
       ///diem giua man hinh
        SDL_Rect gameOverScoreRect = { (SCREEN_WIDTH - textSurface->w) / 2, SCREEN_HEIGHT / 2-60, textSurface->w, textSurface->h };
        SDL_RenderCopy(gfx.renderer, gameOverScoreTexture, NULL, &gameOverScoreRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(gameOverScoreTexture);
    }
} else {
    for (const auto& obstacle : obstacles) {
        obstacle.render(gfx);
    }
    render(mouse, gfx);
    if (scoreTexture) {
        SDL_RenderCopy(gfx.renderer, scoreTexture, NULL, &scoreRect);
    }
}


        gfx.presentScene();
        SDL_Delay(16);
    }

    /// don dep
    Mix_FreeMusic(bgm);
    Mix_FreeMusic(gameOverMusic);
    Mix_FreeChunk(jumpSound);
    Mix_CloseAudio();
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    gfx.quit();
    return 0;
}
