#include<SDL.h>

class Animation {
    private:
        int animFrame;
        int frameCounter;
        int speedCount;
        int totalFrames;
    public:
        Animation();
        void setSpeed(int speed);
        void setTotalFrames(int total);
        int getAnimFrame();
        void reset();
        void update(bool isMoving);
        SDL_Rect getSourceRect(int currentRow, int textureWidth, int textureHeight, int sheetCols = 4, int sheetRows = 4);
    };