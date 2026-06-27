#pragma once
#include <SDL.h>
#include <vector>
#include "Player.h"
#include "Bomb.h"
#include "Map.h"
#include "Enemy.h"
class Game {
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Map* gameMap;
    Player player;
    std::vector<Bomb> bombs;
    std::vector<Enemy> enemies;
    int resetTimer;
public:
    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void spawnEnemiesRandomly(int count);
    bool running() { return isRunning; }
};