#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"
#include "HideBlock.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	std::unique_ptr<DebugCamera> debugCamera_;
	Mouse* mouse = Mouse::GetInstance();
	HideBlock hideBlock_;
	HideBlock hideBlock2_;
	HideBlock hideBlock3_;
	int num_;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};