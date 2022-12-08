#pragma once
#include "Sprite.h"
#include "timer/Timer.h"
#include <vector>
#include "WinApp.h"

class AnimationManager
{
private:
	std::vector<std::vector<Sprite*>> sprite_;
	std::vector<Timer> timer_;
	std::vector<uint32_t> frame_;
public:
	void SetSprite(uint32_t textureHandle, uint32_t frameNum, int frameTime, Vector2 textureSize = { WinApp::kWindowWidth,WinApp::kWindowHeight });
	void Update(int index);
	void Draw(int index);
};