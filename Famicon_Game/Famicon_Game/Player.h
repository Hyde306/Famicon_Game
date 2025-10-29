#pragma once
#include "DxLib.h"
#include "config.h"

enum Direction {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
};

class Player
{
public:
    void Init();
    void Update(int map[MAP_HEIGHT][MAP_WIDTH]);
    void Draw(float scrollX);

    int GetMapX() const;
    int GetMapY() const;

    float GetWorldX() const;
    float GetWorldY() const;
    float GetCenterX() const;  // ← 宣言だけ！
    float GetCenterY() const;  // ← 宣言だけ！

private:
    int mapX, mapY;
    float centerX, centerY;
    int currentFrame;
    int frameTimer;
    int dir;
    int moveDir;
    int image;
};
