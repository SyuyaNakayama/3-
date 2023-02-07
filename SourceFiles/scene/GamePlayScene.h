#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"
#include "HideBlock.h"
#include "Sprite.h"
#include "Skydome.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	std::unique_ptr<DebugCamera> debugCamera_;
	Mouse* mouse = Mouse::GetInstance();
	HideBlock hideBlock_;

	uint16_t textureHandle_ = 0;
	Sprite* UI = nullptr;
	Sprite* back = nullptr;
	Skydome* skydome_ = nullptr;
	
public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};