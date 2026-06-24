#include "Game.h"

bool Game::init() {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Bomberman",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 0
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    isRunning = true;

    player.init(renderer, 100, 100);

    return true;
}

void Game::handleEvents() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT)
            isRunning = false;
        
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                bombs.push_back(Bomb(renderer, player.getX(), player.getY()));
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    player.handleInput(state);
}
void Game::update() {
    player.update();
    
    for (auto it = bombs.begin(); it != bombs.end(); ) {
        it->update();
        if (it->isExploded()) {
            it->clean();
            it = bombs.erase(it);
        } else {
            it++;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    for (auto& b : bombs)
        b.render(renderer);
        
    player.render(renderer);
    
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    for (auto& b : bombs) {
        b.clean();
    }
    bombs.clear();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}