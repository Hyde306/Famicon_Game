#pragma once
#include "config.h"

struct Bomb {
    bool active;       // 有効フラグ
    int mapX, mapY;    // マップ上のマス座標
    float worldX, worldY; // ワールド座標（スクロールに依存しない）
    int timer;         // 爆発までのタイマー
    int currentFrame;  // アニメーションフレーム
    int frameTimer;    // フレーム制御用
};