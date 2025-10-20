#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"
#include "config.h"

// 画像ハンドル
int wallImage;
int floorImage;
int softBlockImage;
int playerImage;

// プレイヤー状態
int currentFrame = 0;
int frameTimer = 0;
int moveTimer = 0;

int playerMapX = 1;
int playerMapY = 1;

float playerDrawX = playerMapX * TILE_SIZE;
float playerDrawY = playerMapY * TILE_SIZE;

float scrollX = 0.0f;

enum Direction { DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
Direction playerDir = DIR_DOWN;

// マップデータ
int map[13][31] = {
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

// コンストラクタ
CGame::CGame(CManager* p) : CScene(p)
{
    floorImage = LoadGraph("image/Ground.png");
    wallImage = LoadGraph("image/Block.png");
    softBlockImage = LoadGraph("image/Break.png");
    playerImage = LoadGraph("image/Player.png");
}

// 更新処理
int CGame::Update()
{
    UpdateScroll();

    bool isKeyPressed = false;
    moveTimer++;

    int nextX = playerMapX;
    int nextY = playerMapY;

    if (CheckHitKey(KEY_INPUT_A)) {
        nextX--;
        playerDir = DIR_LEFT;
        isKeyPressed = true;
    }
    else if (CheckHitKey(KEY_INPUT_D)) {
        nextX++;
        playerDir = DIR_RIGHT;
        isKeyPressed = true;
    }
    else if (CheckHitKey(KEY_INPUT_W)) {
        nextY--;
        playerDir = DIR_UP;
        isKeyPressed = true;
    }
    else if (CheckHitKey(KEY_INPUT_S)) {
        nextY++;
        playerDir = DIR_DOWN;
        isKeyPressed = true;
    }

    if (isKeyPressed && moveTimer >= MOVE_INTERVAL) {
        if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
            int tile = map[nextY][nextX];
            if (tile == 0) {
                playerMapX = nextX;
                playerMapY = nextY;
                moveTimer = 0;
            }
        }
    }

    // プレイヤーの表示座標を滑らかに補間
    float targetX = playerMapX * TILE_SIZE;
    float targetY = playerMapY * TILE_SIZE;
    playerDrawX += (targetX - playerDrawX) * 0.2f;
    playerDrawY += (targetY - playerDrawY) * 0.2f;

    // アニメーション更新
    if (isKeyPressed) {
        frameTimer++;
        if (frameTimer >= FRAME_INTERVAL) {
            frameTimer = 0;
            currentFrame = (currentFrame + 1) % 3;
        }
    }
    else {
        currentFrame = 1;
    }

    for (int i = 0; i < base.size(); i++)
        base[i]->Action(base);

    for (auto i = base.begin(); i != base.end();)
        (*i)->FLAG ? i++ : i = base.erase(i);

    ObjSort_Quick(base, 0, base.size() - 1);

    return 0;
}

// スクロール処理
void CGame::UpdateScroll()
{
    float targetScrollX = playerDrawX - (WINDOW_WIDTH / 2) + (TILE_SIZE / 2);
    scrollX += (targetScrollX - scrollX) * 0.2f;

    float maxScroll = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    if (scrollX < 0) scrollX = 0;
    if (scrollX > maxScroll) scrollX = maxScroll;
}

// マップ描画
void CGame::DrawMap()
{
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int drawX = x * TILE_SIZE - (int)scrollX;
            int drawY = y * TILE_SIZE;
            if (drawX + TILE_SIZE < 0 || drawX > WINDOW_WIDTH) continue;

            switch (map[y][x]) {
            case 0: DrawGraph(drawX, drawY, floorImage, TRUE); break;
            case 1: DrawGraph(drawX, drawY, wallImage, TRUE); break;
            case 2: DrawGraph(drawX, drawY, softBlockImage, TRUE); break;
            }
        }
    }
}

// 描画処理
void CGame::Draw()
{
    ClearDrawScreen();
    DrawMap();

    int srcX = currentFrame * FRAME_WIDTH;
    int srcY = (playerDir == DIR_LEFT || playerDir == DIR_DOWN) ? 0 : FRAME_HEIGHT;
    if (playerDir == DIR_DOWN || playerDir == DIR_UP) srcX += FRAME_WIDTH * 3;

    int playerX = (int)playerDrawX - (int)scrollX;
    int playerY = (int)playerDrawY;

    DrawRectGraph(
        playerX, playerY,
        srcX, srcY,
        FRAME_WIDTH, FRAME_HEIGHT,
        playerImage, TRUE
    );

    DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());

    for (int i = 0; i < base.size(); i++)
        if (base[i]->FLAG) base[i]->Draw();

    ScreenFlip();
}

// デストラクタ
CGame::~CGame()
{
    // 必要なら画像の解放処理を追加
    // DeleteGraph(floorImage);
    // DeleteGraph(wallImage);
    // DeleteGraph(softBlockImage);
    // DeleteGraph(playerImage);
}
