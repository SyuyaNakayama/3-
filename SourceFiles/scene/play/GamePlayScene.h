#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"
#include "Model.h"


class GamePlayScene : public BaseScene
{
private:
	Player player_;
	DebugCamera* debugCamera_ = nullptr;
	Mouse* mouse = Mouse::GetInstance();
	float angle = 0;

	Model* hideBlock = nullptr;

	WorldTransform worldTransform_;

	bool isDraw = true;

public:
	void SetB(Vector3 vec3);

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};