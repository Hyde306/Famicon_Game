//�Q�[���V�[���w�b�_
#pragma once
#include "objBase.h"
#include "CCamera.h"

class CGame :public CScene
{
private:
	//int scrollX = 0; // �X�N���[���ʁi�s�N�Z���P�ʁj

public:

	vector<unique_ptr<BaseVector>> base;

	//�R���X�g���N�^
	CGame(CManager*);

	//�f�X�g���N�^
	~CGame();

	int Update();//�X�V����
	void Draw();//�`�揈��

	int tileImage;
	void UpdateScroll();

	float scrollX = 0.0f;
};