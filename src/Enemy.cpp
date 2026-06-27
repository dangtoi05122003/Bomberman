#include "Enemy.h"
#include "Map.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

Enemy::Enemy() {
    x = 0; y = 0;
    dx = 0; dy = 0;
    speed = 2; 
    isAlive = true;
    enemySpriteSheet = nullptr;
    currentDirectionRow = 2; 
    moveTimer = 0;
    dieTimer = 0;
}

void Enemy::init(SDL_Renderer* r, int px, int py) {
    x = (px / 48) * 48;
    y = (py / 48) * 48;
    isAlive = true;
    currentDirectionRow = 2;
    moveTimer = 0;
    dx = 0; dy = 0;
    
    enemySpriteSheet = IMG_LoadTexture(r, "assets/images/enemy.png");
    if (enemySpriteSheet == nullptr) {
        SDL_Log("Loi: Khong the doc file anh enemy.png: %s", IMG_GetError());
    }
    
    walkAnim.setSpeed(10);      
    walkAnim.setTotalFrames(3); 
}

void Enemy::update(Map* gameMap, int playerX, int playerY, const std::vector<Bomb>& bombs) {
    if (!isAlive) {
        if (dieTimer > 0) {
            dieTimer--;
        }
        return; 
    }
    int eTileX = (x + 24) / 48;
    int eTileY = (y + 24) / 48;
    if (x % 48 == 0 && y % 48 == 0) {
        
        bool hunting = false;
        int targetDx = 0;
        int targetDy = 0;

        int pTileX = (playerX + 24) / 48;
        int pTileY = (playerY + 24) / 48;

        const int VISION_RANGE = 4; 
        int distX = std::abs(eTileX - pTileX);
        int distY = std::abs(eTileY - pTileY);

        auto isBombAt = [&](int tx, int ty) {
            for (const auto& b : bombs) {
                if (b.getX() / 48 == tx && b.getY() / 48 == ty) return true;
            }
            return false;
        };
        if ((eTileX == pTileX && distY <= VISION_RANGE) || (eTileY == pTileY && distX <= VISION_RANGE)) {
            int startX = std::min(eTileX, pTileX);
            int endX = std::max(eTileX, pTileX);
            int startY = std::min(eTileY, pTileY);
            int endY = std::max(eTileY, pTileY);
            bool blocked = false;

            if (eTileY == pTileY) {
                for (int i = startX + 1; i < endX; i++) {
                    SDL_Rect r = { i * 48, eTileY * 48, 48, 48 };
                    if ((gameMap && gameMap->isWall(r)) || isBombAt(i, eTileY)) { blocked = true; break; }
                }
            } else {
                for (int i = startY + 1; i < endY; i++) {
                    SDL_Rect r = { eTileX * 48, i * 48, 48, 48 };
                    if ((gameMap && gameMap->isWall(r)) || isBombAt(eTileX, i)) { blocked = true; break; }
                }
            }
            if (!blocked) {
                hunting = true;
                if (eTileX < pTileX) { targetDx = speed;  targetDy = 0;  currentDirectionRow = 3; } 
                if (eTileX > pTileX) { targetDx = -speed; targetDy = 0;  currentDirectionRow = 1; } 
                if (eTileY < pTileY) { targetDx = 0;  targetDy = speed;  currentDirectionRow = 2; } 
                if (eTileY > pTileY) { targetDx = 0;  targetDy = -speed; currentDirectionRow = 0; } 
            }
        }
        if (hunting) {
            dx = targetDx;
            dy = targetDy;
            moveTimer = 0; 
        } 
        else {
            moveTimer--;
            int dirs[4][2] = {{0, -speed}, {-speed, 0}, {0, speed}, {speed, 0}};
            std::vector<int> validIndices;
            for (int i = 0; i < 4; i++) {
                int nextTileX = eTileX + (dirs[i][0] / speed);
                int nextTileY = eTileY + (dirs[i][1] / speed);
                SDL_Rect testRect = { nextTileX * 48 + 4, nextTileY * 48 + 4, 40, 40 };

                if (gameMap && !gameMap->isWall(testRect) && !isBombAt(nextTileX, nextTileY)) {
                    validIndices.push_back(i);
                }
            }
            bool hasIntersection = false;
            for (int idx : validIndices) {
                if ((dx != 0 && dirs[idx][1] != 0) || (dy != 0 && dirs[idx][0] != 0)) {
                    hasIntersection = true;
                    break;
                }
            }
            if (moveTimer <= 0 || (dx == 0 && dy == 0) || hasIntersection) {
                moveTimer = 30 + rand() % 40; 

                if (!validIndices.empty()) {
                    int chosenIdx = validIndices[rand() % validIndices.size()];
                    dx = dirs[chosenIdx][0];
                    dy = dirs[chosenIdx][1];
                    currentDirectionRow = chosenIdx;
                } else {
                    dx = 0; dy = 0;
                }
            }
        }
    }
    x += dx;
    y += dy;
    SDL_Rect enemyRect = { x + 4, y + 4, 40, 40 };
    if (gameMap && gameMap->isWall(enemyRect)) {
        x -= dx;
        y -= dy;
        dx = 0;
        dy = 0;
        moveTimer = 0;
        x = eTileX * 48;
        y = eTileY * 48;
    }

    walkAnim.update(true);
}

void Enemy::render(SDL_Renderer* r) {
    if (!isAlive && dieTimer <= 0) return;
    SDL_Rect destRect = { x, y, 48, 48 };

    if (enemySpriteSheet != nullptr) {
        int textureWidth, textureHeight;
        SDL_QueryTexture(enemySpriteSheet, NULL, NULL, &textureWidth, &textureHeight);
        
        int cellWidth = textureWidth / 4;
        int cellHeight = textureHeight / 4;
        SDL_Rect srcRect;

        if (!isAlive) {
            srcRect.x = 3 * cellWidth;
            srcRect.y = 0 * cellHeight;
            srcRect.w = cellWidth;
            srcRect.h = cellHeight;
        } else {
            srcRect = walkAnim.getSourceRect(currentDirectionRow, textureWidth, textureHeight);
            if (srcRect.x >= 3 * cellWidth) {
                srcRect.x = 0; 
            }
        }

        SDL_RenderCopy(r, enemySpriteSheet, &srcRect, &destRect);
    } else {
        if (isAlive) SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
        else SDL_SetRenderDrawColor(r, 100, 100, 100, 255);
        SDL_RenderFillRect(r, &destRect);
    }
}

void Enemy::clean() {
    if (enemySpriteSheet != nullptr) {
        SDL_DestroyTexture(enemySpriteSheet);
        enemySpriteSheet = nullptr;
    }
}