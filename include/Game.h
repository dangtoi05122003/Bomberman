#pragma once
#include <SDL.h>
#include <vector>
#include "Player.h"
#include "Bomb.h"

class Game {
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Player player;
    std::vector<Bomb> bombs;

public:
    bool init();
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() { return isRunning; }
};