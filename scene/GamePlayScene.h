#pragma once
#include "BaseScene.h"
#include "Audio.h"
#include "Model.h"
#include "player/Player.h"
#include "collider/CollisionManager.h"
#include "DebugCamera.h"
#include "BlockManager.h"

class GamePlayScene : public BaseScene
{
private:
	Model* model_;
	CollisionManager collisionManager;
	Player* player_ = Player::GetInstance();
	DebugCamera* debugCamera_ = nullptr;
	BlockManager* blockManager = BlockManager::GetInstance();
	UINT16 stage = 0;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};