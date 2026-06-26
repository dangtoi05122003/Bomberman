#pragma once
#include <SDL.h>
#include <SDL_image.h>
class Map;
class Bomb {
private:
    int x, y;
    int timer;
    SDL_Texture* textures[12]; 
    int explosionFrame;
    int delayCounter;
    bool playerInside;
public:
    Bomb(SDL_Renderer* r, int px, int py); 
    void update();
    void render(SDL_Renderer* r, Map* gameMap);
    void clean(); 
    bool isExploded() { return timer <= 0 && explosionFrame >= 8; }
    int getX() { return x; }
    int getY() { return y; }
    bool isPlayerInside() { return playerInside; }
    void setPlayerOutside() { playerInside = false; }
    bool isExploding() { return timer <= 0 && explosionFrame < 8; }
};