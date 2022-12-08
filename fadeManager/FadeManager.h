#pragma once
#include "Sprite.h"
#include "enum.h"

class FadeManager
{
private:
	const int FADE_TIME = 60;
	float fadePerFlame = 1.0f / FADE_TIME;
	Sprite* sprite_ = nullptr;
	uint32_t texture_ = 0;
	bool isFade_ = 0;
	float alpha_ = 0;
	Scene* nowScene_ = nullptr;
	Scene nextScene_ = Title;

public:
	void Initialize(Scene* nowScene);
	void Update();
	void Draw();

	void FadeScene();
	void ChangeScene(Scene nextScene);
};