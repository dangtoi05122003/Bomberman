#include "Map.h"
#include <SDL_image.h>
#include <cstdlib> 
#include <ctime>   
#include <iostream>
using namespace std;
Map::Map(): rows(0), cols(0), size(48) {
    wallTexture = nullptr;
    boxTexture = nullptr;
    grassTexture = nullptr;
}
Map::~Map() {
    if(wallTexture) SDL_DestroyTexture(wallTexture);
    if(boxTexture) SDL_DestroyTexture(boxTexture);
    if(grassTexture) SDL_DestroyTexture(grassTexture);
}
void Map::loadTextures(SDL_Renderer* renderer) {
    wallTexture = IMG_LoadTexture(renderer, "assets/images/wall.png");
    boxTexture = IMG_LoadTexture(renderer, "assets/images/box.png");
    grassTexture = IMG_LoadTexture(renderer, "assets/images/grass.png");

    if (!wallTexture || !boxTexture || !grassTexture) {
        cerr << "Lỗi: Kiểm tra lại các file ảnh wall.png, brick.png, grass.png trong assets/images/!" << std::endl;
    }
}
bool Map::generateRandomMap(int r, int c) {
    rows = r;
    cols = c;
    map.resize(rows, vector<char>(cols, '.'));
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
                map[i][j] = '#'; 
            } 
            else if (i % 2 == 0 && j % 2 == 0) {
                map[i][j] = '#'; 
            }
            else {
                if ((i == 1 && j == 1) || (i == 1 && j == 2) || (i == 2 && j == 1)) {
                    map[i][j] = '.'; 
                    continue;
                }
                if (rand() % 100 < 40) {
                    map[i][j] = 'B';
                }
            }
        }
    }
    return true;
}

void Map::render(SDL_Renderer* renderer) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            SDL_Rect destRect = { j * size, i * size, size, size };
            if (grassTexture) {
                SDL_RenderCopy(renderer, grassTexture, nullptr, &destRect);
            }
            if (map[i][j] == '#' && wallTexture) {
                SDL_RenderCopy(renderer, wallTexture, nullptr, &destRect);
            } 
            else if (map[i][j] == 'B' && boxTexture) {
                SDL_RenderCopy(renderer, boxTexture, nullptr, &destRect);
            }
        }
    }
}

bool Map::isWall(int x, int y) {
    int tileX = x / size;
    int tileY = y / size;
    if (tileX < 0 || tileX >= cols || tileY < 0 || tileY >= rows) return true;
    return map[tileY][tileX] == '#' || map[tileY][tileX] == 'B';
}

bool Map::isWall(SDL_Rect rect) {
    int leftTile   = rect.x / size;
    int rightTile  = (rect.x + rect.w - 1) / size;
    int topTile    = rect.y / size;
    int bottomTile = (rect.y + rect.h - 1) / size;

    if (leftTile < 0 || rightTile >= cols || topTile < 0 || bottomTile >= rows) {
        return true; 
    }

    for (int i = topTile; i <= bottomTile; ++i) {
        for (int j = leftTile; j <= rightTile; ++j) {
            if (map[i][j] == '#' || map[i][j] == 'B') {
                return true;
            }
        }
    }
    return false;
}
void Map::destroyBox(SDL_Rect rect) {
    int col = rect.x / size;
    int row = rect.y / size;
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        if (map[row][col] == 'B') { 
            map[row][col] = '.'; 
        }
    }
}