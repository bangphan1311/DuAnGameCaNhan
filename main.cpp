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

///trang thai
enum GameState {
    STATE_START_SCREEN,
    STATE_PLAYING,
    STATE_GAME_OVER
};

int main(int argc, char* argv[]) {
    Graphics gfx;
    gfx.init();

    if (TTF_Init() == -1) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
    }

    TTF_Font* font = TTF_OpenFont("ARIBL0.ttf", 40);



    Mix_Music* bgm = Mix_LoadMUS("energetic-chiptune-video-game-music-platformer-8-bit-318348.mp3");
    Mix_Music* gameOverMusic = Mix_LoadMUS("hieu_ung_am_thanh_mat_mang-www_tiengdong_com.mp3");
    Mix_Chunk* jumpSound = Mix_LoadWAV("hieu_ung_am_thanh_nhay_qua_buoc_qua-www_tiengdong_com (mp3cut.net).mp3");


/// add hinh anh
    SDL_Texture* obstacleTex1 = gfx.loadTexture("bt-removebg-preview.png");
    SDL_Texture* obstacleTex2 = gfx.loadTexture("rắn-removebg-preview.png");
    SDL_Texture* obstacleTex3 = gfx.loadTexture("gầ-removebg-preview.png");

    SDL_Texture* obstacleTex1Night = gfx.loadTexture("mandem2-removebg-preview.png");
    SDL_Texture* obstacleTex2Night = gfx.loadTexture("mandem3-removebg-preview.png");
    SDL_Texture* obstacleTex3Night = gfx.loadTexture("mandem1-removebg-preview.png");

    SDL_Texture* obstacleTex1Winter = gfx.loadTexture("result_w22.png");
    SDL_Texture* obstacleTex2Winter = gfx.loadTexture("winter1.png");
    SDL_Texture* obstacleTex3Winter = gfx.loadTexture("w3.png");
    SDL_Texture* mouseTexture = gfx.loadTexture("shin.png");
    SDL_Texture* mouseTextureNight = gfx.loadTexture("shintoi.png");
    SDL_Texture* mouseTextureWinter = gfx.loadTexture("muadong.png");

    SDL_Texture* background = gfx.loadTexture("bground.jpg");
    SDL_Texture* backgroundNight = gfx.loadTexture("dem.jpg");
    SDL_Texture* backgroundWinter = gfx.loadTexture("donggg.jpg");

    SDL_Texture* gameOverTexture = gfx.loadTexture("0veer.png");
    /// man hinh start
    SDL_Texture* startScreenTexture = gfx.loadTexture("startttt.png");

    std::vector<Obstacle> obstacles;
    obstacles.push_back(Obstacle(obstacleTex1, 500, GROUND_Y + 5, 70, 70, 1));
    obstacles.push_back(Obstacle(obstacleTex2, 1000, GROUND_Y - 10, 80, 85, 2));
    obstacles.push_back(Obstacle(obstacleTex3, 2500, GROUND_Y - 8, 80, 80, 3));

    Mouse mouse(mouseTexture, 67, 76, 6, 100, GROUND_Y - 45, jumpSound);

    int score = 0;
    Uint32 lastScoreTime = SDL_GetTicks();
    SDL_Color textColor = {255, 255, 255};
    SDL_Texture* scoreTexture = nullptr;
    SDL_Rect scoreRect;

    bool running = true;
    bool gameOver = false;
    bool nightModeActivated = false;
    bool winterModeActivated = false;
    SDL_Texture* currentBackground = background;
    SDL_Texture* currentMouseTexture = mouseTexture;
    std::vector<SDL_Texture*> currentObstacleTextures = {obstacleTex1, obstacleTex2, obstacleTex3};

    int bgX = 0;
    float currentSpeed = 6.0f;
    Uint32 lastSpeedIncrease = SDL_GetTicks();

    ///bat dau
    GameState gameState = STATE_START_SCREEN;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            ///man hinh start
            if (gameState == STATE_START_SCREEN && event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_s) {
                    gameState = STATE_PLAYING;
                    Mix_PlayMusic(bgm, -1); ///chay nhac
                }
            } else if (gameState == STATE_PLAYING && event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_SPACE)
                    mouse.jump();
            }
///game over va replay , khoi phuc su vat
            if (gameState == STATE_GAME_OVER && event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                gameState = STATE_PLAYING;
                gameOver = false;
                score = 0;
                currentSpeed = 5.0f;
                bgX = 0;
                nightModeActivated = false;
                winterModeActivated = false;
                lastScoreTime = SDL_GetTicks();
                lastSpeedIncrease = SDL_GetTicks();

                Mix_HaltMusic();
                Mix_PlayMusic(bgm, -1);

                mouse.reset();
                mouse.texture = mouseTexture;
                currentMouseTexture = mouseTexture;
                currentBackground = background;
                obstacles.clear();
                obstacles.push_back(Obstacle(obstacleTex1, 500, GROUND_Y + 5, 70, 70, 1));
                obstacles.push_back(Obstacle(obstacleTex2, 1000, GROUND_Y - 10, 80, 85, 2));
                obstacles.push_back(Obstacle(obstacleTex3, 2500, GROUND_Y - 8, 80, 80, 3));
                currentObstacleTextures = {obstacleTex1, obstacleTex2, obstacleTex3};
///hien thi diem khi thua
                if (scoreTexture) {
                    SDL_DestroyTexture(scoreTexture);
                    scoreTexture = nullptr;
                }
            }
        }

        gfx.prepareScene();

       /// hien thi man hinh start
        if (gameState == STATE_START_SCREEN) {
            SDL_RenderCopy(gfx.renderer, startScreenTexture, NULL, NULL);
        } else if (gameState == STATE_PLAYING) {
            gfx.renderScrollingBackground(currentBackground, bgX);

            mouse.update();
            mouse.texture = currentMouseTexture;/// cap nhat shin
            for (size_t i = 0; i < obstacles.size(); ++i) {
                obstacles[i].update(currentSpeed);
                obstacles[i].texture = currentObstacleTextures[i]; /// cap nhat chuowng ngai vat
                if (checkCollision(mouse, obstacles[i])) {
                    gameState = STATE_GAME_OVER;
                    Mix_HaltMusic();
                    Mix_PlayMusic(gameOverMusic, -1);
                }
            }
///tang toc moi 5s
            if (SDL_GetTicks() - lastSpeedIncrease > 5000) {
                currentSpeed = std::min(currentSpeed + 0.5f, 15.0f);
                lastSpeedIncrease = SDL_GetTicks();
            }

            bgX -= (int)currentSpeed;
            if (bgX <= -SCREEN_WIDTH) bgX = 0;

            if (SDL_GetTicks() - lastScoreTime > 500) {
                score++;///diem tang them moi nua giay
                lastScoreTime = SDL_GetTicks();
                std::string scoreText = "SCORE: " + std::to_string(score);///HIEN THI DIEM
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
                ///ban ngay
                if (textSurface) {
                    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
                    scoreTexture = SDL_CreateTextureFromSurface(gfx.renderer, textSurface);
                    scoreRect = {SCREEN_WIDTH - textSurface->w - 20, 20, textSurface->w, textSurface->h};
                    SDL_FreeSurface(textSurface);
                }
                ///ban dem

                if (score >= 10 && !nightModeActivated && !winterModeActivated) {
                    currentBackground = backgroundNight;
                    currentMouseTexture = mouseTextureNight;
                    currentObstacleTextures = {obstacleTex1Night, obstacleTex2Night, obstacleTex3Night};
                    nightModeActivated = true;
                }
                ///mua dong

                if (score >= 15 && !winterModeActivated) {
                    currentBackground = backgroundWinter;
                    currentMouseTexture = mouseTextureWinter;
                    currentObstacleTextures = {obstacleTex1Winter, obstacleTex2Winter, obstacleTex3Winter};
                    winterModeActivated = true;
                    nightModeActivated = false;
                }
            }

            for (const auto& obstacle : obstacles) obstacle.render(gfx);
            render(mouse, gfx);
            if (scoreTexture) SDL_RenderCopy(gfx.renderer, scoreTexture, NULL, &scoreRect);
        } else if (gameState == STATE_GAME_OVER) {
            SDL_RenderCopy(gfx.renderer, gameOverTexture, NULL, NULL);
            std::string scoreText = "Score: " + std::to_string(score);///score game over
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
            ///ve gameover
            if (textSurface) {
                SDL_Texture* gameOverScoreTexture = SDL_CreateTextureFromSurface(gfx.renderer, textSurface);
                SDL_Rect rect = {(SCREEN_WIDTH - textSurface->w) / 2, SCREEN_HEIGHT / 2 - 60, textSurface->w, textSurface->h};
                SDL_RenderCopy(gfx.renderer, gameOverScoreTexture, NULL, &rect);

                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(gameOverScoreTexture);
            }
        }

        gfx.presentScene();
        SDL_Delay(16);
    }

    Mix_FreeMusic(bgm);
    Mix_FreeMusic(gameOverMusic);
    Mix_FreeChunk(jumpSound);
    Mix_CloseAudio();

    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (startScreenTexture) SDL_DestroyTexture(startScreenTexture);///xoa cai man hinh start
    ///giai phong bo nho
    SDL_DestroyTexture(backgroundWinter);
    SDL_DestroyTexture(obstacleTex1Winter);
    SDL_DestroyTexture(obstacleTex2Winter);
    SDL_DestroyTexture(obstacleTex3Winter);
    SDL_DestroyTexture(mouseTextureWinter);
    SDL_DestroyTexture(backgroundNight);
    SDL_DestroyTexture(obstacleTex1Night);
    SDL_DestroyTexture(obstacleTex2Night);
    SDL_DestroyTexture(obstacleTex3Night);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(obstacleTex1);
    SDL_DestroyTexture(obstacleTex2);
    SDL_DestroyTexture(obstacleTex3);
    SDL_DestroyTexture(mouseTexture);
    SDL_DestroyTexture(mouseTextureNight);
    SDL_DestroyTexture(gameOverTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    gfx.quit();

    return 0;
}
