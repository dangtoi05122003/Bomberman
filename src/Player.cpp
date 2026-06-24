#include "Player.h"

void Player::init(SDL_Renderer* r, int px, int py) {
    x = px;
    y = py;
    playerSpriteSheet = IMG_LoadTexture(r, "assets/images/player.png");
    if (playerSpriteSheet == nullptr) {
        SDL_Log("Loi khong the nap anh player.png: %s", IMG_GetError());
    }
    currentDirectionRow = 2; 
}

void Player::handleInput(const Uint8* state) {
    if (state[SDL_SCANCODE_W]) {
        y -= speed;
        currentDirectionRow = 0;
    }
    if (state[SDL_SCANCODE_S]) {
        y += speed;
        currentDirectionRow = 2;
    }
    if (state[SDL_SCANCODE_A]) {
        x -= speed;
        currentDirectionRow = 1;
    }
    if (state[SDL_SCANCODE_D]) {
        x += speed;
        currentDirectionRow = 3;
    }
}

void Player::update() {}

void Player::render(SDL_Renderer* r) {
    SDL_Rect destRect = { x, y, 48, 48 }; 

    if (playerSpriteSheet != nullptr) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(playerSpriteSheet, NULL, NULL, &textureWidth, &textureHeight);
        int cellWidth = textureWidth / 4;
        int cellHeight = textureHeight / 4;
        SDL_Rect srcRect = { 0, currentDirectionRow * cellHeight, cellWidth, cellHeight };
        
        SDL_RenderCopy(r, playerSpriteSheet, &srcRect, &destRect);
    } else {
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderFillRect(r, &destRect);
    }
}

void Player::clean() {
    if (playerSpriteSheet != nullptr) {
        SDL_DestroyTexture(playerSpriteSheet);
    }
}