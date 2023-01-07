#pragma once
#include "BaseScene.h"

enum class Scene { Null, Title, Play, GameOver, Clear };

class SceneFactory
{
private:
	SceneFactory() = default;

public:
	SceneFactory(const SceneFactory& obj) = delete;
	static SceneFactory* GetInstance();

	BaseScene* CreateScene(Scene scene);
};