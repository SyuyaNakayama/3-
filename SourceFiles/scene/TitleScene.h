#pragma once
#include "BaseScene.h"
#include "Timer.h"
#include <memory>

class TitleScene : public BaseScene
{
private:
	float cameraUpAngle = 0;
	Vector3 upVector = { 0,0,1 };
	float upRotSpd = 0.005f;
	bool isNextScene = false;
	Timer timer = 60;
	std::unique_ptr<Sprite> sprite;
	Timer animeTimer = 30;
	UINT16 animeFrame = 0;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
};