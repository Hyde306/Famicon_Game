#pragma once
#include"DxLib.h"
#include "config.h"

// Player.h の上部などに追加
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

    // 🔽 これを追加！
    float GetCenterX() const { return centerX; }
    float GetWorldX() const;
    float GetWorldY() const;

private:
    int mapX, mapY;
    float centerX, centerY;
    int currentFrame;
    int frameTimer;
    int dir;
    int moveDir;
    int image;
};