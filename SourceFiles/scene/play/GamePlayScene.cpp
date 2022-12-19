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
	blockManager->Update();
	player_->Update();

	// �����蔻��
	collisionManager.CheckAllCollisions();

	debugCamera_->Update();
	//*viewProjection_ = debugCamera_->GetViewProjection();
}

void GamePlayScene::Draw()
{
	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete model_;
}