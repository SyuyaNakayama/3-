#pragma once

#include <vector>
#include "BaseScene.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "FadeManager.h"
#include "SceneFactory.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
public: // メンバ関数
	~GameScene();

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

	void SetNextScene(Scene nextScene, bool isUseFade = true);
	static GameScene* GetInstance();
	GameScene(const GameScene& obj) = delete;
	static UINT16* GetStage() { return &stage; }
private: // メンバ変数
	GameScene() = default;

	FadeManager fadeManager_;
	BaseScene* scene_ = nullptr;
	Scene nextScene_ = Scene::Null;
	SceneFactory* sceneFactory_ = SceneFactory::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	Input* input = Input::GetInstance();
	Audio* audio = Audio::GetInstance();
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	static UINT16 stage;
};