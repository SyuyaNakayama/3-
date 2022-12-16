#include "TitleScene.h"
#include "TextureManager.h"
#include "GamePlayScene.h"
#include "GameScene.h"

void TitleScene::Initialize()
{
	sprite = Sprite::Create(TextureManager::Load("Unnamed (13).png"), {});
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
		GamePlayScene* nextScene = new GamePlayScene;
		gameScene->SetNextScene(nextScene);
	}
}

void TitleScene::Draw()
{
	Sprite::PreDraw(cmdList);
	sprite->Draw();
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
}