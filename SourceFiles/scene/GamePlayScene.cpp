#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"

void GamePlayScene::Initialize()
{
	gameScene = GameScene::GetInstance();

	viewProjection->eye.y = 10.0f;
	viewProjection->target.y = 10.0f;

	model_ = Model::Create();

	blockManager->Initialize(stage);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_->Initialize();
}

void GamePlayScene::Update()
{
	player_->Update();

	// 当たり判定
	collisionManager.CheckAllCollisions(player_);


	blockManager->Update();


	debugCamera_->Update();
	//*viewProjection_ = debugCamera_->GetViewProjection();
}

void GamePlayScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Model::PreDraw(cmdList);

	blockManager->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete model_;
}