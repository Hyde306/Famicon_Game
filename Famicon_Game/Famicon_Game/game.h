//ゲームシーンヘッダ
#pragma once
#include "objBase.h"
#include "CCamera.h"

class CGame :public CScene
{
private:

public:

	vector<unique_ptr<BaseVector>> base;

	//コンストラクタ
	CGame(CManager*);

	//デストラクタ
	~CGame();

	int Update();//更新処理
	void Draw();//描画処理

	int tileImage;
	void UpdateScroll();

	float scrollX = 0.0f;
};