#include "Player.h"
#include "config.h"
#include <cmath>

bool CanMove(float centerX, float centerY, int map[MAP_HEIGHT][MAP_WIDTH]) {
    const int COLLISION_WIDTH = 60;
    const int COLLISION_HEIGHT = 56;

    int left = (int)(centerX - COLLISION_WIDTH / 2) / TILE_SIZE;
    int right = (int)(centerX + COLLISION_WIDTH / 2 - 1) / TILE_SIZE;
    int top = (int)(centerY - COLLISION_HEIGHT / 2) / TILE_SIZE;
    int bottom = (int)(centerY + COLLISION_HEIGHT / 2 - 1) / TILE_SIZE;

    return map[top][left] == 0 &&
        map[top][right] == 0 &&
        map[bottom][left] == 0 &&
        map[bottom][right] == 0;
}

void Player::Init() {
    mapX = 1;
    mapY = 1;
    centerX = mapX * TILE_SIZE + TILE_SIZE / 2;
    centerY = mapY * TILE_SIZE + TILE_SIZE / 2;
    currentFrame = frameTimer = 0;
    dir = DIR_DOWN;
    moveDir = -1;
    image = LoadGraph("image/Player.png"); // 384Å~192Åi64Å~64 Å~ 6Å~3Åj
}

void Player::Update(int map[MAP_HEIGHT][MAP_WIDTH]) {
    if (CheckHitKey(KEY_INPUT_A)) moveDir = DIR_LEFT;
    else if (CheckHitKey(KEY_INPUT_D)) moveDir = DIR_RIGHT;
    else if (CheckHitKey(KEY_INPUT_W)) moveDir = DIR_UP;
    else if (CheckHitKey(KEY_INPUT_S)) moveDir = DIR_DOWN;
    else moveDir = -1;

    const float SPEED = MOVE_INTERVAL;
    float nextX = centerX;
    float nextY = centerY;

    switch (moveDir) {
    case DIR_LEFT:  nextX -= SPEED; dir = DIR_LEFT;  break;
    case DIR_RIGHT: nextX += SPEED; dir = DIR_RIGHT; break;
    case DIR_UP:    nextY -= SPEED; dir = DIR_UP;    break;
    case DIR_DOWN:  nextY += SPEED; dir = DIR_DOWN;  break;
    }

    if (moveDir != -1 && CanMove(nextX, nextY, map)) {
        centerX = nextX;
        centerY = nextY;
    }

    mapX = (int)(centerX) / TILE_SIZE;
    mapY = (int)(centerY) / TILE_SIZE;

    float centerTargetX = mapX * TILE_SIZE + TILE_SIZE / 2;
    float centerTargetY = mapY * TILE_SIZE + TILE_SIZE / 2;
    const float SNAP_DISTANCE = 2.0f;

    if (fabs(centerX - centerTargetX) < SNAP_DISTANCE) centerX = centerTargetX;
    if (fabs(centerY - centerTargetY) < SNAP_DISTANCE) centerY = centerTargetY;

    if (moveDir != -1) {
        frameTimer++;
        if (frameTimer >= FRAME_INTERVAL) {
            frameTimer = 0;
            currentFrame = (currentFrame + 1) % 3;
        }
    }
    else {
        currentFrame = 1;
    }
}

void Player::Draw(float scrollX) {
    int srcX = 0;
    int srcY = 0;

    switch (dir) {
    case DIR_LEFT:
        srcX = currentFrame * FRAME_WIDTH;
        srcY = 0;
        break;

    case DIR_DOWN:
        srcX = (currentFrame + 3) * FRAME_WIDTH;
        srcY = 0;
        break;

    case DIR_RIGHT:
        srcX = currentFrame * FRAME_WIDTH;
        srcY = FRAME_HEIGHT;
        break;

    case DIR_UP:
        srcX = (currentFrame + 3) * FRAME_WIDTH;
        srcY = FRAME_HEIGHT;
        break;
    }

    int x = (int)(centerX - FRAME_WIDTH / 2 - scrollX);
    int y = (int)(centerY - FRAME_HEIGHT / 2);

    DrawRectGraph(x, y, srcX, srcY, FRAME_WIDTH, FRAME_HEIGHT, image, TRUE);
}

int Player::GetMapX() const { return mapX; }
int Player::GetMapY() const { return mapY; }
