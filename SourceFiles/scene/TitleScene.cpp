#include "TitleScene.h"
#include "TextureManager.h"
#include "GamePlayScene.h"
#include "GameScene.h"

void TitleScene::Initialize()
{
	gameScene = GameScene::GetInstance();
}

void TitleScene::Finalize()
{
	delete sprite;
}

void TitleScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		gameScene->SetNextScene(Scene::Play);
	}
}

void TitleScene::Draw()
{
	Sprite::PreDraw(cmdList);
	//sprite->Draw();
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
}