#pragma once
#include "config.h"

struct Bomb {
    bool active;       // �L���t���O
    int mapX, mapY;    // �}�b�v��̃}�X���W
    float worldX, worldY; // ���[���h���W�i�X�N���[���Ɉˑ����Ȃ��j
    int timer;         // �����܂ł̃^�C�}�[
    int currentFrame;  // �A�j���[�V�����t���[��
    int frameTimer;    // �t���[������p
};