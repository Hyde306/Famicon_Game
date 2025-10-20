#pragma once

constexpr int TILE_SIZE = 64;
constexpr int MAP_WIDTH = 31;  // �}�b�v�̉����i�X�N���[���\�j
constexpr int MAP_HEIGHT = 13;

constexpr int WINDOW_WIDTH = TILE_SIZE * 20; // �E�B���h�E
constexpr int WINDOW_HEIGHT = TILE_SIZE * MAP_HEIGHT; // 896px

// �v���C���[���
constexpr int FRAME_WIDTH = 64;
constexpr int FRAME_HEIGHT = 64;
constexpr int FRAME_INTERVAL = 5;
constexpr int MOVE_INTERVAL = 10;