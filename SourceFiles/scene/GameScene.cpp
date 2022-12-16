#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "GamePlayScene.h"
#include "TitleScene.h"
#include "Sprite.h"
#include "WinApp.h"

void GameScene::Initialize() 
{
	// 入力の初期化
	input->Initialize();

	// オーディオの初期化
	audio->Initialize();

	dxCommon->Initialize(WinApp::GetInstance());

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator->Initialize();

	primitiveDrawer->Initialize();

	viewProjection->Initialize();

	// シーンの生成
	//scene_ = new GamePlayScene;
	scene_ = new GamePlayScene;
	scene_->Initialize();
	imguiManager->Initialize();
	fadeManager_.Initialize();
}

void GameScene::Update()
{
	// 入力関連の毎フレーム処理
	input->Update();
	
	imguiManager->Begin();
	// ゲームシーンの毎フレーム処理
	fadeManager_.Update();
	
	if (fadeManager_.IsChange() || !scene_)
	{
		if (nextScene_)
		{
			if (scene_)
			{
				scene_->Finalize();
				delete scene_;
			}

			scene_ = nextScene_;
			nextScene_ = nullptr;
			scene_->Initialize();
		}
	}

	if (!fadeManager_.IsFade()) { scene_->Update(); }

	imguiManager->End();

	viewProjection->UpdateMatrix();
	// 軸表示の更新
	axisIndicator->Update();
}

void GameScene::Draw()
{
	// 描画開始
	dxCommon->PreDraw();
	// ゲームシーンの描画
	scene_->Draw();
	// 軸表示の描画
	axisIndicator->Draw();
	// プリミティブ描画のリセット
	primitiveDrawer->Reset();
	fadeManager_.Draw();
	// デバッグの描画(ImGui)
	imguiManager->Draw();
	// 描画終了
	dxCommon->PostDraw();
}

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::SetNextScene(BaseScene* nextScene, bool isUseFade)
{
	nextScene_ = nextScene;
	if (isUseFade) { fadeManager_.FadeScene(); }
}
