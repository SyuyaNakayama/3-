#include "TitleScene.h"
#include "TextureManager.h"

void TitleScene::Initialize()
{
	sprite = Sprite::Create(TextureManager::Load("Unnamed (13).png"), {});
}

void TitleScene::Finalize()
{
	delete sprite;
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
	//Sprite::PreDraw(cmdList);
	sprite->Draw();
	Sprite::PostDraw();

	DirectXCommon::GetInstance()->ClearDepthBuffer();
}