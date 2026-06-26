#include "Game.h"

bool Game::init() {
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
    return true;
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
        if (it->isExploding() && player.getIsAlive()) {
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
                    if (SDL_HasIntersection(&playerRect, &explosionZones[i])) {
                        player.kill();
                        break;
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
    if (!player.getIsAlive()) {
        resetTimer++;
        if (resetTimer >= 180) { 
            resetTimer = 0;
            for (auto& b : bombs) {
                b.clean();
            }
            bombs.clear();
            if (gameMap) {
                gameMap->generateRandomMap(13, 15);
            }
            player.init(renderer, 48, 48);
        }
    } else {
        resetTimer = 0;
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