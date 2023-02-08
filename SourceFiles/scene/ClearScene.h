#pragma once
#include "BaseScene.h"
#include "Timer.h"

class ClearScene : public BaseScene
{
private:
	float cameraUpAngle = 0;
	Vector3 upVector = { 0,0,1 };
	float upRotSpd = 0.005f;
	bool isNextScene = false;
	Timer timer = 60;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};