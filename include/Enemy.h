#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Animation.h"
#include "Bomb.h"

class Map;

class Enemy {
private:
    int x, y;
    int dx, dy;
    int speed;
    bool isAlive;
    int dieTimer;
    SDL_Texture* enemySpriteSheet;
    Animation walkAnim;       
    int currentDirectionRow;  
    int moveTimer;            

public:
    Enemy();
    void init(SDL_Renderer* r, int px, int py);
    void update(Map* gameMap, int playerX, int playerY, const std::vector<Bomb>& bombs);
    void render(SDL_Renderer* r);
    void clean();
    
    int getX() const { return x; }
    int getY() const { return y; }
    bool getIsAlive() const { return isAlive; }
    void kill() { 
        if (isAlive) {
            isAlive = false; 
            dx = 0;
            dy = 0;
            dieTimer = 60;
        }
    }
    bool shouldRemove() const { return !isAlive && dieTimer <= 0; }
};