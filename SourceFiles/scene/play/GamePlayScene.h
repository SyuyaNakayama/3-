#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	DebugCamera* debugCamera_ = nullptr;
	Mouse* mouse = Mouse::GetInstance();
	float angle = 0;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};