#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

bool Game::init() {
    std::srand(std::time(nullptr));
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Bomberman",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        720, 624, 0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    isRunning = true;
    gameMap = new Map();
    gameMap->generateRandomMap(13, 15);
    gameMap->loadTextures(renderer);
    player.init(renderer, 48, 48);
    resetTimer = 0;
    spawnEnemiesRandomly(2);
    return true;
}
void Game::spawnEnemiesRandomly(int count) {
    for (auto& e : enemies) {
        e.clean();
    }
    enemies.clear();
    if (!gameMap) return;
    std::vector<std::pair<int, int>> emptyTiles;
    for (int row = 2; row < 12; row++) {
        for (int col = 2; col < 14; col++) {
            SDL_Rect tileRect = { col * 48, row * 48, 48, 48 };
            if (!gameMap->isWall(tileRect)) {
                if (std::abs(col - 1) > 2 || std::abs(row - 1) > 2) {
                    emptyTiles.push_back({col, row});
                }
            }
        }
    }
    if (!emptyTiles.empty()) {
        for (int i = 0; i < count && !emptyTiles.empty(); i++) {
            int randomIndex = std::rand() % emptyTiles.size();
            auto chosenTile = emptyTiles[randomIndex];
            Enemy newEnemy;
            newEnemy.init(renderer, chosenTile.first * 48, chosenTile.second * 48);
            enemies.push_back(newEnemy);
            emptyTiles.erase(emptyTiles.begin() + randomIndex);
        }
    }
}
void Game::handleEvents() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            isRunning = false;
        
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE && player.getIsAlive()) {
                int pX = player.getX();
                int pY = player.getY();
                int snappedX = (pX + 24) / 48 * 48;
                int snappedY = (pY + 24) / 48 * 48;
                bombs.push_back(Bomb(renderer, snappedX, snappedY));
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    player.handleInput(state);
}
void Game::update() {
    int oldX = player.getX();
    int oldY = player.getY();
    player.update();
    SDL_Rect playerRect = { player.getX() + 12, player.getY() + 12, 24, 24 };
    for(auto& b : bombs) {
        if (!b.isExploded() && !b.isExploding()) {
            SDL_Rect bombRect = { b.getX(), b.getY(), 48, 48 };
            bool isOverlapping = SDL_HasIntersection(&playerRect, &bombRect);
            if (b.isPlayerInside()) {
                if (!isOverlapping) {
                    b.setPlayerOutside();
                }
            }
            else {
                if (isOverlapping) {
                    player.setX(oldX);
                    player.setY(oldY);
                    playerRect = { oldX + 12, oldY + 12, 24, 24 }; 
                }
            }
        }
    }
    if (gameMap) {
        if (gameMap->isWall(playerRect)) {
            player.setX(oldX);
            player.setY(oldY);
            playerRect = { oldX + 12, oldY + 12, 24, 24 };
        }
    }
    for (auto it = bombs.begin(); it != bombs.end(); ) {
        it->update();
        if (it->isExploding()) {
            int bX = it->getX();
            int bY = it->getY();
            SDL_Rect explosionZones[5] = {
                { bX, bY, 48, 48 },
                { bX - 48, bY, 48, 48 },
                { bX + 48, bY, 48, 48 },
                { bX, bY - 48, 48, 48 },
                { bX, bY + 48, 48, 48 }
            };
            if (gameMap) {
                for (int i = 0; i < 5; i++) {
                    if (i > 0 && gameMap->isWall(explosionZones[i])) {
                        continue; 
                    }
                    if (player.getIsAlive() && SDL_HasIntersection(&playerRect, &explosionZones[i])) {
                        player.kill();
                        break;
                    }
                    for (auto& e : enemies) {
                        if (e.getIsAlive()) {
                            SDL_Rect enemyHitbox = { e.getX() + 4, e.getY() + 4, 40, 40 };
                            if (SDL_HasIntersection(&enemyHitbox, &explosionZones[i])) {
                                e.kill(); 
                            }
                        }
                    }
                }
            }
        }
        if (it->isExploded()) {
            it->clean();
            it = bombs.erase(it);
        } else {
            it++;
        }
    }
    for (auto& e : enemies) {
        e.update(gameMap, player.getX(), player.getY(), bombs);
        if (e.getIsAlive() && player.getIsAlive()) {
            SDL_Rect enemyRect = { e.getX() + 6, e.getY() + 6, 36, 36 };
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                player.kill();
            }
        }
    }
    if (!player.getIsAlive() || enemies.empty()) {
        resetTimer++;
        if (resetTimer >= 180) { 
            resetTimer = 0;
            for (auto& b : bombs) {
                b.clean();
            }
            bombs.clear();
            for (auto& e : enemies) {
                e.clean();
            }
            enemies.clear();
            if (gameMap) {
                gameMap->generateRandomMap(13, 15);
            }
            player.init(renderer, 48, 48);
            spawnEnemiesRandomly(2);
        }
    } else {
        resetTimer = 0;
    }
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->shouldRemove()) {
            it->clean();
            it = enemies.erase(it);
        } else {
            it++;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    if(gameMap) {
        gameMap-> render(renderer);
    }
    for (auto& b : bombs)
        b.render(renderer, gameMap);
    for (auto& e : enemies) {
        e.render(renderer);
    }
    player.render(renderer);
    
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    if(gameMap) {
        delete gameMap;
        gameMap = nullptr;
    }
    for (auto& b : bombs) {
        b.clean();
    }
    bombs.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}