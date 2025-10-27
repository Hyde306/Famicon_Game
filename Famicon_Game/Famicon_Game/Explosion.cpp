#include "Explosion.h"
#include "DxLib.h"

extern int explosionImg;

void InitExplosion(Explosion& e)
{
    e.active = false;
    e.timer = 0;
    e.currentFrame = 0;
    e.frameTimer = 0;
    e.mapX = e.mapY = 0;
}

void DrawExplosion(Explosion& e, float scrollX)
{
    if (!e.active) return;

    int frameWidth = 256 / 4;  // â°4ÉRÉ}
    int frameHeight = 64;

    int srcX = e.currentFrame * frameWidth;
    int srcY = 0;

    int x = e.mapX * TILE_SIZE - (int)scrollX;
    int y = e.mapY * TILE_SIZE;

    DrawRectExtendGraph(x, y, x + TILE_SIZE, y + TILE_SIZE,
        srcX, srcY, frameWidth, frameHeight, explosionImg, TRUE);
}
