#include "Player.h"

void Player::init(SDL_Renderer* r, int px, int py) {
    x = px;
    y = py;
    dx = 0;
    dy = 0;
    isAlive = true;
    playerSpriteSheet = IMG_LoadTexture(r, "assets/images/player.png");
    if (playerSpriteSheet == nullptr) {
        SDL_Log("Loi khong the nap anh player.png: %s", IMG_GetError());
    }
    currentDirectionRow = 2;
    walkAnim.setSpeed(8);
    walkAnim.setTotalFrames(3);
}

void Player::handleInput(const Uint8* state) {
    if(!isAlive) return;
    dx = 0;
    dy = 0;
    if (state[SDL_SCANCODE_W]) {
        dy -= speed;
        currentDirectionRow = 0;
    }
    if (state[SDL_SCANCODE_S]) {
        dy = speed;
        currentDirectionRow = 2;
    }
    if (state[SDL_SCANCODE_A]) {
        dx -= speed;
        currentDirectionRow = 1;
    }
    if (state[SDL_SCANCODE_D]) {
        dx = speed;
        currentDirectionRow = 3;
    }
}

void Player::update() {
    if (!isAlive) return;
    x += dx;
    y += dy;
    bool isMoving = (dx != 0 || dy != 0);
    walkAnim.update(isMoving);
}

void Player::render(SDL_Renderer* r) {
    SDL_Rect destRect = { x, y, 48, 48 }; 

    if (playerSpriteSheet != nullptr) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(playerSpriteSheet, NULL, NULL, &textureWidth, &textureHeight);
        SDL_Rect srcRect;
        if  (!isAlive) {
            int cellWidth = textureWidth / 4;
            int cellHeight = textureHeight / 4;
            srcRect= {3 * cellWidth, 0 * cellHeight, cellWidth, cellHeight };
        }
        else {
            srcRect = walkAnim.getSourceRect(currentDirectionRow, textureWidth, textureHeight);
        }
        
        SDL_RenderCopy(r, playerSpriteSheet, &srcRect, &destRect);
    } else {
        if (isAlive) {
            SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(r, 150, 150, 150, 255);
        }
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
        SDL_RenderFillRect(r, &destRect);
    }
}

void Player::clean() {
    if (playerSpriteSheet != nullptr) {
        SDL_DestroyTexture(playerSpriteSheet);
        playerSpriteSheet = nullptr;
    }
}