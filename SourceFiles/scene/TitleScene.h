#pragma once
#include "Sprite.h"
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private:
	Sprite* sprite = nullptr;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};