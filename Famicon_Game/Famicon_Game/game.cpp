#include "DxLib.h"
#include "Scene_Manager.h"
#include "game.h"
#include "function.h"
#include "config.h"
#include "Map.h"
#include "Player.h"

Player player;
float scrollX = 0.0f;

CGame::CGame(CManager* p) : CScene(p)
{
    InitMapGraphics();
    player.Init();
}

int CGame::Update()
{
    UpdateScroll();
    player.Update(map);

    for (int i = 0; i < base.size(); i++)
        base[i]->Action(base);

    for (auto i = base.begin(); i != base.end();)
        (*i)->FLAG ? i++ : i = base.erase(i);

    ObjSort_Quick(base, 0, base.size() - 1);
    return 0;
}

void CGame::UpdateScroll()
{
    float targetScrollX = player.GetMapX() * TILE_SIZE - (WINDOW_WIDTH / 2) + (TILE_SIZE / 2);
    scrollX += (targetScrollX - scrollX) * 0.2f;

    float maxScroll = MAP_WIDTH * TILE_SIZE - WINDOW_WIDTH;
    if (scrollX < 0) scrollX = 0;
    if (scrollX > maxScroll) scrollX = maxScroll;
}

void CGame::Draw()
{
    ClearDrawScreen();
    DrawMap(scrollX);
    player.Draw(scrollX);

    DrawFormatString(0, 0, GetColor(255, 255, 255), "Object_Count = %d", base.size());

    for (int i = 0; i < base.size(); i++)
        if (base[i]->FLAG) base[i]->Draw();

    ScreenFlip();
}

CGame::~CGame() {}
