#pragma once
#include "config.h"

struct Explosion {
    int mapX;
    int mapY;
    int timer;         // 表示時間
    bool active;
    int currentFrame;  // アニメーションコマ
    int frameTimer;    // フレーム進行
};
