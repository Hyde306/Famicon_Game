//Map.cpp
#include "Map.h"
#include"DxLib.h"

int map[MAP_HEIGHT][MAP_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,2,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,2,0,2,0,0,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,2,1,2,1,0,1,0,1,0,1,0,1,0,1,2,1,2,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,2,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,2,1,0,1,0,1,2,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,2,0,0,0,0,2,2,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// �摜�n���h���i���[�J���ɕێ��j
static int wallImage;
static int floorImage;
static int softBlockImage;

// �������i�摜�ǂݍ��݁j
void InitMapGraphics() {
    floorImage = LoadGraph("image/Ground.png");
    wallImage = LoadGraph("image/Block.png");
    softBlockImage = LoadGraph("image/Break.png");
}

// �}�b�v�`��
void DrawMap(float scrollX)
{
    for (int y = 0; y < MAP_HEIGHT; y++) 
    {
        for (int x = 0; x < MAP_WIDTH; x++) 
        {
            int drawX = x * TILE_SIZE - (int)scrollX;
            int drawY = y * TILE_SIZE;
            if (drawX + TILE_SIZE < 0 || drawX > WINDOW_WIDTH) continue;

            switch (map[y][x])
            {
            case 0: DrawGraph(drawX, drawY, floorImage, TRUE); break;
            case 1: DrawGraph(drawX, drawY, wallImage, TRUE); break;
            case 2: DrawGraph(drawX, drawY, softBlockImage, TRUE); break;
            }
        }
    }
}
