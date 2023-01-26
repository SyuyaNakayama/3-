#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	std::unique_ptr<DebugCamera> debugCamera_;
	Mouse* mouse = Mouse::GetInstance();

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};