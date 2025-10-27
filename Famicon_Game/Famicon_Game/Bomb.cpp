#include "Bomb.h"

// ‰Šú‰»
void InitBomb(Bomb& bomb)
{
    bomb.active = false;
    bomb.mapX = 0;
    bomb.mapY = 0;
    bomb.timer = 0;
    bomb.currentFrame = 0;
    bomb.frameTimer = 0;
}
