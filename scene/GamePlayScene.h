#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Audio.h"
#include "Model.h"
#include "player/Player.h"
#include "collider/CollisionManager.h"
#include "DebugCamera.h"

class GamePlayScene : public BaseScene
{
private:
	std::vector<WorldTransform> blocks;
	bool isDrag = false;
	Model* model_;
	CollisionManager collisionManager;
	Player* player_ = Player::GetInstance();
	DebugCamera* debugCamera_ = nullptr;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};