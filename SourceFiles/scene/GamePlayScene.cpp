#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "TitleScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"

void GamePlayScene::Initialize()
{
	gameScene = GameScene::GetInstance();

	viewProjection->eye = { 0,0,-1200 };
	//viewProjection->eye = { 1200,0,0 };
	viewProjection->up = { 0,1,0 };

	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_.Initialize();
	
	mouse->Initialize();
	blockManager->Initialize(1);
	//blockManager->Initialize(2);
	//blockManager->Initialize(3);
	//blockManager->Initialize(4);
	//blockManager->Initialize(5);
}

void GamePlayScene::Update()
{
	mouse->Update();
	blockManager->Update();
	player_.Update();

	// 当たり判定
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

#ifdef _DEBUG
	//*viewProjection = debugCamera_->GetViewProjection(); 
#endif // _DEBUG
}

void GamePlayScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_.Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}