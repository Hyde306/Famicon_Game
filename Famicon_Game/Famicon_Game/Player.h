//Player.h
#pragma once
#include "DxLib.h"
#include "config.h"

enum Direction { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };

class Player {
private:
    int mapX, mapY;
    float centerX, centerY;
    int dir;
    int moveDir;
    int image;
    int currentFrame;
    int frameTimer;

public:
    void Init();
    void Update(int map[MAP_HEIGHT][MAP_WIDTH]);
    void Draw(float scrollX);
    int GetMapX() const;
    int GetMapY() const;
};
