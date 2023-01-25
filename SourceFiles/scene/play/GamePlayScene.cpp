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
	viewProjection->farZ = 15000.0f;

	blockManager->Initialize(stage);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_.Initialize();

	mouse->Initialize();
}

void GamePlayScene::Update()
{
	mouse->Update();
	blockManager->Update();
	player_.Update();

	static float angle = 0;
	Quaternion rotaQ = { std::cos(angle / 2.0f) ,Vector3(0,1,0) * std::sin(angle / 2.0f) };
	viewProjection->eye = Quaternion::RotateVector(Vector3(70, -40.0f, -1200), rotaQ);
	angle += 0.05f;
	imguiManager->PrintVector("viewProjection->eye", viewProjection->eye);
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