#pragma once
#include "config.h"

struct Explosion {
    int mapX;
    int mapY;
    int timer;         // �\������
    bool active;
    int currentFrame;  // �A�j���[�V�����R�}
    int frameTimer;    // �t���[���i�s
};
