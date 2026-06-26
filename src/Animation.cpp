#include "Animation.h"

Animation::Animation() {
    animFrame = 0;
    frameCounter = 0;
    speedCount = 8;
    totalFrames = 3;
}
void Animation::setSpeed(int speed) {
    speedCount = speed;
}
void Animation::setTotalFrames(int total) { 
    totalFrames = total; 
}
int Animation::getAnimFrame() { 
    return animFrame; 
}
void Animation::reset() {
    animFrame = 0;
    frameCounter = 0;
}
void Animation::update(bool isMoving) {
    if (isMoving) {
        frameCounter++;
        if (frameCounter >= speedCount) {
            animFrame = (animFrame + 1) % totalFrames;
            frameCounter = 0;
        }
    } else {
        animFrame = 0;
    }
}
SDL_Rect Animation::getSourceRect(int currentRow, int textureWidth, int textureHeight, int sheetCols, int sheetRows) {
    int cellWidth = textureWidth / sheetCols;
    int cellHeight = textureHeight / sheetRows;
    return { animFrame * cellWidth, currentRow * cellHeight, cellWidth, cellHeight };
}