#pragma once
#include "Sprite.h"

class FadeManager
{
private:
	const int FADE_TIME = 30;
	float fadePerFlame = 1.0f / FADE_TIME;
	Sprite* sprite = nullptr;
	bool isFade = 0;
	float alpha = 0;
	bool isChange = false;

public:
	void Initialize();
	void Update();
	void Draw();
	bool IsChange() { return isChange; }
	bool IsFade() { return isFade; }
	void FadeScene() { isFade = true; }
};