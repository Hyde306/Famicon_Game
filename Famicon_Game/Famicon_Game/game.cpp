#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"
#include "config.h"
#include "Map.h"
#include "Player.h"
#include "Bomb.h"
#include "Explosion.h"
#include "BreakEffect.h"

// グローバル変数
Player player;

Bomb bomb;
Explosion explosions[9];   // 中心＋上下左右＋中間

int bombImg;
int explosionImg;

BreakEffect breakEffects[16]; // 最大16個くらい同時発生可能
int breakImg; // 破壊エフェクト画像

// コンストラクタ
CGame::CGame(CManager* p) : CScene(p)
{
    InitMapGraphics();
    player.Init();

    // 爆弾初期化
    bomb.active = false;
    bomb.mapX = 0;
    bomb.mapY = 0;
    bomb.timer = 0;
    bomb.currentFrame = 0;
    bomb.frameTimer = 0;

    // 爆発初期化
    for (int i = 0; i < 9; i++) 
    {
        explosions[i].active = false;
        explosions[i].mapX = 0;
        explosions[i].mapY = 0;
        explosions[i].timer = 0;
        explosions[i].currentFrame = 0;
        explosions[i].frameTimer = 0;
    }

    // 画像読み込み
    bombImg = LoadGraph("image/Bomb.png");           // 3コマ横（透過済み）
    explosionImg = LoadGraph("image/Explosion.png"); // 256×256（4コマアニメ）
}

int CGame::Update()
{
    player.Update(map);
    UpdateScroll();

    // ボム設置（スペースキー）
    static bool prevSpace = false;
    bool nowSpace = CheckHitKey(KEY_INPUT_SPACE);

    if (nowSpace && !prevSpace && !bomb.active)
    {
        bomb.active = true;

        bomb.mapX = player.GetMapX();
        bomb.mapY = player.GetMapY();

        bomb.timer = 180;
        bomb.currentFrame = 0;
        bomb.frameTimer = 0;
    }


    prevSpace = nowSpace;

    // ボム進行
    if (bomb.active)
    {
        bomb.timer--;
        bomb.frameTimer++;

        // アニメーション切り替え
        if (bomb.frameTimer >= 15)
        {
            bomb.frameTimer = 0;
            bomb.currentFrame = (bomb.currentFrame + 1) % 3;
        }

        // 爆発発生
        if (bomb.timer <= 0)
        {
            bomb.active = false;

            int dx[4] = { 0, 0, -1, 1 };
            int dy[4] = { -1, 1, 0, 0 };

            // 中心の爆風
            explosions[0].active = true;
            explosions[0].mapX = bomb.mapX;
            explosions[0].mapY = bomb.mapY;
            explosions[0].timer = 60;
            explosions[0].currentFrame = 0;
            explosions[0].frameTimer = 0;

            // 各方向へ伸ばす（爆風の長さ＝2）
            int index = 1;
            for (int dir = 0; dir < 4; dir++) 
            {
                for (int len = 1; len <= 2; len++)
                {
                    int nx = bomb.mapX + dx[dir] * len;
                    int ny = bomb.mapY + dy[dir] * len;

                    // 範囲外
                    if (nx < 0 || ny < 0 || nx >= MAP_WIDTH || ny >= MAP_HEIGHT)
                        break;

                    // 壊せないブロック → 爆風を止める
                    if (map[ny][nx] == 1)
                        break;

                    // 壊せるブロック → ブロック破壊して爆風もそこで止める
                    if (map[ny][nx] == 2) 
                    {
                        // 壊れエフェクトを出す
                        for (int e = 0; e < 16; e++)
                        {
                            if (!breakEffects[e].active) 
                            {
                                breakEffects[e].active = true;
                                breakEffects[e].worldX = nx * TILE_SIZE;
                                breakEffects[e].worldY = ny * TILE_SIZE;
                                breakEffects[e].timer = 30;        // 表示時間
                                breakEffects[e].frameTimer = 0;
                                breakEffects[e].currentFrame = 0;
                                break;
                            }
                        }

                        // ブロック破壊
                        map[ny][nx] = 0;
                        break;
                    }

                    // 通過できるマス → 爆風を生成
                    explosions[index].active = true;
                    explosions[index].mapX = nx;
                    explosions[index].mapY = ny;
                    explosions[index].timer = 60;
                    explosions[index].currentFrame = 0;
                    explosions[index].frameTimer = 0;
                    index++;
                }
            }
        }
    }

    // 爆風アニメ進行
    for (int i = 0; i < 9; i++)
    {
        if (!explosions[i].active) continue;

        explosions[i].timer--;
        explosions[i].frameTimer++;

        // 4コマアニメ（0〜3）
        if (explosions[i].frameTimer >= 10) 
        {
            explosions[i].frameTimer = 0;
            explosions[i].currentFrame++;

            if (explosions[i].currentFrame >= 4)
            {
                explosions[i].active = false;
                continue;
            }
        }

        if (explosions[i].timer <= 0)
            explosions[i].active = false;
    }

    for (int i = 0; i < 16; i++) 
    {
        if (!breakEffects[i].active) continue;

        breakEffects[i].timer--;
        breakEffects[i].frameTimer++;

        // 6フレームごとに次のコマへ（6枚アニメ）
        if (breakEffects[i].frameTimer >= 6)
        {
            breakEffects[i].frameTimer = 0;
            breakEffects[i].currentFrame++;
        }

        if (breakEffects[i].timer <= 0 || breakEffects[i].currentFrame >= 6)
            breakEffects[i].active = false;
    }

    // 既存のオブジェクト処理（敵など）
    for (int i = 0; i < base.size(); i++)
        base[i]->Action(base);

    for (auto i = base.begin(); i != base.end();)
        (*i)->FLAG ? i++ : i = base.erase(i);

    ObjSort_Quick(base, 0, base.size() - 1);

    return 0;
}

// スクロール制御
void CGame::UpdateScroll()
{
    float playerCenter = player.GetCenterX();
    float targetScrollX = player.GetCenterX() - (WINDOW_WIDTH / 2.0f);
    scrollX += (targetScrollX - scrollX) * 0.2f;
    float maxScroll = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    if (scrollX < 0) scrollX = 0;
    if (scrollX > maxScroll) scrollX = maxScroll;

}

// 爆弾描画
void CGame::DrawBomb()
{
    if (!bomb.active) return;

    int frameWidth = 814 / 3;
    int frameHeight = 272;
    int srcX = bomb.currentFrame * frameWidth;
    int srcY = 0;

    float drawX = bomb.mapX * TILE_SIZE - scrollX;  // ← this->scrollX でOK
    float drawY = bomb.mapY * TILE_SIZE;

    DrawRectExtendGraph(
        (int)drawX,
        (int)drawY,
        (int)(drawX + TILE_SIZE),
        (int)(drawY + TILE_SIZE),
        srcX, srcY, frameWidth, frameHeight,
        bombImg, TRUE
    );
}


// 爆発描画
void CGame::DrawExplosion()
{
    const int frameWidth = 128;
    const int frameHeight = 128;

    for (int i = 0; i < 9; i++)
    {
        if (!explosions[i].active) continue;

        int frame = explosions[i].currentFrame;
        if (frame > 3) frame = 3;

        int srcX = (frame % 2) * frameWidth;
        int srcY = (frame / 2) * frameHeight;

        //  まず world座標を計算（マップ上の絶対位置）
        float worldX = explosions[i].mapX * TILE_SIZE;
        float worldY = explosions[i].mapY * TILE_SIZE;

        //  そこからスクロール補正して画面上に描画
        float screenX = worldX - scrollX;
        float screenY = worldY;

        DrawRectExtendGraph(
            (int)screenX, (int)screenY,
            (int)(screenX + TILE_SIZE), (int)(screenY + TILE_SIZE),
            srcX, srcY, frameWidth, frameHeight,
            explosionImg, TRUE
        );
    }
}

void DrawBreakEffects(float scrollX)
{
    const int frameWidth = 575 / 6;
    const int frameHeight = 96;       // 高さ

    for (int i = 0; i < 16; i++) {
        if (!breakEffects[i].active) continue;

        int frame = breakEffects[i].currentFrame;
        if (frame > 5) frame = 5; // 全6コマ

        int srcX = frame * frameWidth;
        int srcY = 0;

        float drawX = breakEffects[i].worldX - scrollX;
        float drawY = breakEffects[i].worldY;

        DrawRectExtendGraph(
            (int)drawX, (int)drawY,
            (int)(drawX + TILE_SIZE), (int)(drawY + TILE_SIZE),
            srcX, srcY, frameWidth, frameHeight,
            breakImg, TRUE
        );
    }
}

// 描画処理
void CGame::Draw()
{
    ClearDrawScreen();

    // 背景やマップを先に描画
    DrawMap(scrollX);

    // 爆弾と爆発を描画
    DrawBomb();
    DrawExplosion();

    // ブロック破壊エフェクトをここで描画
    DrawBreakEffects(scrollX);

    // プレイヤーを最後に描画
    player.Draw(scrollX);

    DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());

    for (int i = 0; i < base.size(); i++)
        if (base[i]->FLAG) base[i]->Draw();

    ScreenFlip();
}

CGame::~CGame() {}
