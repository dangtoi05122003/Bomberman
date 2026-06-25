#include "Bomb.h"
#include "Map.h"
#include <string>

Bomb::Bomb(SDL_Renderer* r, int px, int py) {
    x = px;
    y = py;
    timer = 180;
    explosionFrame = 0;
    delayCounter = 0;
    playerInside = true;
    for (int i = 0; i < 12; i++) {
        std::string path = "assets/images/bomb/";
        if (i < 10) {
            path += "00" + std::to_string(i);
        } else {
            path += "0" + std::to_string(i);
        }
        path += ".png";
        textures[i] = IMG_LoadTexture(r, path.c_str());
        if (textures[i] == nullptr) {
            SDL_Log("Khong the tai anh: %s", IMG_GetError());
        }
    }
}

void Bomb::update() {
    if (timer > 0) {
        timer--;
    } else {
        delayCounter++;
        if (delayCounter >= 5) {
            explosionFrame++;
            delayCounter = 0;
        }
    }
}

void Bomb::render(SDL_Renderer* r, Map* gameMap) {
    if (timer <= 0 && explosionFrame >= 8) return; 
    SDL_Texture* currentTexture = nullptr;
    if (timer > 0) {
        int bombFrame = (timer / 15) % 4; 
        currentTexture = textures[bombFrame];
        if (currentTexture != nullptr) {
            SDL_Rect rect = { x, y, 48, 48 };
            SDL_RenderCopy(r, currentTexture, NULL, &rect);
        }
    } else {
        int currentExplosionIndex = 4 + explosionFrame;
        if (currentExplosionIndex < 12) {
            currentTexture = textures[currentExplosionIndex];
        }
        if (currentTexture != nullptr) {
            SDL_Rect centerRect = {x, y, 48, 48};
            SDL_RenderCopy(r, currentTexture, NULL, &centerRect);
            if (gameMap) {
                SDL_Rect leftRect = {x - 48, y, 48, 48};
                gameMap->destroyBox(leftRect);
                if(!gameMap->isWall(leftRect)) {
                    SDL_RenderCopy(r, currentTexture, NULL, &leftRect);
                }
                SDL_Rect rightRect = { x + 48, y, 48, 48 };
                gameMap->destroyBox(rightRect);
                if(!gameMap->isWall(rightRect)) {
                    SDL_RenderCopy(r, currentTexture, NULL, &rightRect);
                }
                SDL_Rect upRect = { x, y - 48, 48, 48 };
                gameMap->destroyBox(upRect);
                if(!gameMap->isWall(upRect)) {
                    SDL_RenderCopy(r, currentTexture, NULL, &upRect);
                }
                SDL_Rect downRect = { x, y + 48, 48, 48 };
                gameMap->destroyBox(downRect);
                if(!gameMap->isWall(downRect)) {
                    SDL_RenderCopy(r, currentTexture, NULL, &downRect);
                }
            }
        }
    }
}
void Bomb::clean() {
    for (int i = 0; i < 12; i++) {
        if (textures[i] != nullptr) {
            SDL_DestroyTexture(textures[i]);
            textures[i] = nullptr;
        }
    }
}