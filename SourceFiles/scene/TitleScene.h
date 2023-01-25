#pragma once
#include "Sprite.h"
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private:
	float cameraUpAngle = 0;
	Vector3 upVector = { 0,0,1 };

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};