#pragma once
#include <SDL.h>
#include <SDL_image.h>
class Bomb {
private:
    int x, y;
    int timer;
    SDL_Texture* textures[12]; 
    int explosionFrame;

public:
    Bomb(SDL_Renderer* r, int px, int py); 
    void update();
    void render(SDL_Renderer* r);
    void clean(); 
    bool isExploded() { return timer <= 0 && explosionFrame >= 8; }
};