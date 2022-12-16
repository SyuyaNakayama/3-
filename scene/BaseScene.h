#pragma once
#include "Input.h"
#include "Audio.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"

class GameScene;

class BaseScene
{
protected:
	GameScene* gameScene = nullptr;
	Input* input = Input::GetInstance();
	Audio* audio = Audio::GetInstance();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

public:
	virtual ~BaseScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};