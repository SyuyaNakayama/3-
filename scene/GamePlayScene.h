#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Audio.h"

class GamePlayScene : public BaseScene
{
private:

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};