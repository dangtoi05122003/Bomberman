#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Player {
    private:
        int x, y;
        int speed = 4;

        SDL_Texture* playerSpriteSheet;
        int currentDirectionRow;

    public:
        void init(SDL_Renderer* r, int px, int py);
        void handleInput(const Uint8* state);
        void update();
        void render(SDL_Renderer* r);
        void clean(); 

        int getX(){ return x; }
        int getY(){ return y; }
};