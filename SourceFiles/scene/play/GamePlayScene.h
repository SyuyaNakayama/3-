#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "BlockManager.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	DebugCamera* debugCamera_ = nullptr;
	BlockManager* blockManager = BlockManager::GetInstance();
	UINT16 stage = 0;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};