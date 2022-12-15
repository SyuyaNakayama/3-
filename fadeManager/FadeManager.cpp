#include "FadeManager.h"
#include "TextureManager.h"
#include "WinApp.h"

void FadeManager::Initialize()
{
	sprite = Sprite::Create(TextureManager::Load("white1x1.png"), {});
	sprite->SetSize({ WinApp::kWindowWidth,WinApp::kWindowHeight });
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