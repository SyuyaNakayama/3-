#include "FadeManager.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "DirectXCommon.h"

void FadeManager::Initialize()
{
	sprite.reset(Sprite::Create(TextureManager::Load("white1x1.png"), {}));
	sprite->SetSize({ WinApp::kWindowWidth,WinApp::kWindowHeight });
	sprite->SetColor({});
}

void FadeManager::Update()
{
	if (!isFade) { return; }
	isChange = false;
	alpha += fadePerFlame;
	if (alpha >= 1.0f)
	{
		alpha = 1.0f;
		fadePerFlame = -fadePerFlame;
		isChange = true;
	}
	if (alpha <= 0)
	{
		alpha = 0;
		fadePerFlame = -fadePerFlame;
		isFade = false;
	}
	sprite->SetColor({ 0,0,0,alpha });
}

void FadeManager::Draw()
{
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite->Draw();
	Sprite::PostDraw();
}