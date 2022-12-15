#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "player/Player.h"
#include "collider/CollisionManager.h"
#include <vector>
#include "ImGuiManager.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
private: // サブクラス
	enum Scene { Title, HowToPlay, Play, Clear, GameOver };
public: // メンバ関数
	~GameScene() { imguiManager->Finalize(); }

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugCamera* debugCamera_ = nullptr;
	Model* model_;
	ViewProjection* viewProjection_ = ViewProjection::GetInstance();
	CollisionManager collisionManager;
	Player player_;
	std::vector<WorldTransform> blocks;
	bool isDrag = false;
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
};