#pragma once
#include "config.h"
#include "DxLib.h"

// �}�b�v�f�[�^ (0:��, 1:��, 2:�󂹂�u���b�N)
extern int map[MAP_HEIGHT][MAP_WIDTH];

// �������i�摜�ǂݍ��݁j
void InitMapGraphics();

// �}�b�v�`��֐�
void DrawMap(float scrollX);
