#ifndef MAP_H
#define MAP_H
#include <SDL.h>
#include <vector>
using namespace std;
class Map {
    private:
        int rows;
        int cols;
        int size;
    vector<vector<char>> map;
    SDL_Texture* wallTexture;
    SDL_Texture* boxTexture;
    SDL_Texture* grassTexture;
    public:
        Map();
        ~Map();
        bool generateRandomMap(int rows, int cols);
        void loadTextures(SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer);
        bool isWall(int x, int y);
        bool isWall(SDL_Rect rect);
        int getSize() const {return size;}
        void destroyBox(SDL_Rect rect);
};
#endif