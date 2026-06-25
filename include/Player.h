#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Player {
    private:
        int x, y;
        int dx = 0;
        int dy = 0;
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
        void setX(int value) { x = value; }
        void setY(int value) { y = value; }
};