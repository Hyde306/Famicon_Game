#pragma once

struct BreakEffect {
    bool active;
    float worldX, worldY; // マップ上の絶対座標
    int currentFrame;
    int frameTimer;
    int timer;
};