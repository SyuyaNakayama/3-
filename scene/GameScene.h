#pragma once

#include <vector>
#include "ImGuiManager.h"
#include "BaseScene.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "fadeManager/FadeManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
public: // メンバ関数
	~GameScene() { scene_->Finalize(), audio->Finalize(); imguiManager->Finalize(); }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetNextScene(BaseScene* nextScene, bool isUseFade = true);
	static GameScene* GetInstance();
	GameScene(const GameScene& obj) = delete;
private: // メンバ変数
	GameScene() = default;

	FadeManager fadeManager_;
	BaseScene* scene_ = nullptr ,* nextScene_ = nullptr;
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	Input* input = Input::GetInstance();
	Audio* audio = Audio::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	AxisIndicator* axisIndicator = AxisIndicator::GetInstance();
	PrimitiveDrawer* primitiveDrawer = PrimitiveDrawer::GetInstance();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
};