#include "Bomb.h"
#include <string>

Bomb::Bomb(SDL_Renderer* r, int px, int py) {
    x = px;
    y = py;
    timer = 180;
    explosionFrame = 0;
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
        static int delayCounter = 0;
        delayCounter++;
        if (delayCounter >= 5) {
            explosionFrame++;
            delayCounter = 0;
        }
    }
}

void Bomb::render(SDL_Renderer* r) {
    if (timer <= 0 && explosionFrame >= 8) return; 
    SDL_Rect rect = { x, y, 32, 32 };
    SDL_Texture* currentTexture = nullptr;
    if (timer > 0) {
        int bombFrame = (timer / 15) % 4; 
        currentTexture = textures[bombFrame];
    } else {
        int currentExplosionIndex = 4 + explosionFrame;
        if (currentExplosionIndex < 12) {
            currentTexture = textures[currentExplosionIndex];
        }
    }
    if (currentTexture != nullptr) {
        SDL_RenderCopy(r, currentTexture, NULL, &rect);
    }
}
void Bomb::clean() {
    for (int i = 0; i < 12; i++) {
        if (textures[i] != nullptr) {
            SDL_DestroyTexture(textures[i]);
        }
    }
}