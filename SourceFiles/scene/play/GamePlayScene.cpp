#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "TitleScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include <DirectXMath.h>

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

	hideBlock = Model::CreateFromOBJ("hideblock");

	worldTransform_.Initialize();

	worldTransform_.scale_ = { 0.5f,0.511f,0.511f };
	worldTransform_.rotation_ = { 0,1.555f,0 };
	//worldTransform_.translation_ = { 19.0f,-19.0f,-1.0f };

	worldTransform_.Update();
}

void GamePlayScene::Update()
{
	SetB(Vector3{19,-19,-1});
	mouse->Update();
	blockManager->Update();
	player_.Update();

	// 当たり判定
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

	//*viewProjection = debugCamera_->GetViewProjection(); 

	if (input->TriggerKey(DIK_SPACE))
	{
		gameScene->SetNextScene(Scene::Title);
	}

	if (input->PushKey(DIK_1))
	{
		worldTransform_.rotation_.x += 0.2f;
		worldTransform_.scale_.y -= 0.01f;
		worldTransform_.scale_.z -= 0.01f;
		worldTransform_.Update();

		if (worldTransform_.scale_.y <= 0.01f)
		{
			isDraw = false;
		}
		
	}
}

void GamePlayScene::Draw()
{
	// 3Dオブジェクト描画前処理
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_.Draw();
	if (isDraw == true)
	{
		hideBlock->Draw(worldTransform_, *ViewProjection::GetInstance());
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
}

void GamePlayScene::Finalize()
{
	delete debugCamera_;
}

void GamePlayScene::SetB(Vector3 vec3)
{
	worldTransform_.translation_ = vec3;
	worldTransform_.Update();
}