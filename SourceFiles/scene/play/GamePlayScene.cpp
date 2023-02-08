#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "TitleScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"

void GamePlayScene::Initialize()
{
	gameScene = GameScene::GetInstance();

	viewProjection->eye = { 70,-40.0f,-1200 };
	viewProjection->target = { 70,-40.0f,0 };
	viewProjection->up = { 0,1,0 };

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_.Initialize();
	
	mouse->Initialize();
	blockManager->Initialize(2);
}

void GamePlayScene::Update()
{
	mouse->Update();
	blockManager->Update();
	player_.Update();

	// �����蔻��
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

	//*viewProjection = debugCamera_->GetViewProjection(); 

	if (input->TriggerKey(DIK_SPACE))
	{
		gameScene->SetNextScene(Scene::Title);
	}
}

void GamePlayScene::Draw()
{
	Sprite::PreDraw(cmdList);
	blockManager->SpriteDraw();
	Sprite::PostDraw();


	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_.Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete debugCamera_;
}