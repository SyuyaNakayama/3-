#pragma once
#include "Input.h"
#include "Audio.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"

class GameScene;

class BaseScene
{
protected:
	GameScene* gameScene = nullptr;
	Input* input = Input::GetInstance();
	Audio* audio = Audio::GetInstance();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	//ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();

public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};