#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "TitleScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include "WinApp.h"
#include "ViewProjection.h"

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

	hideBlock1_.Initialize({ -20,-20,-40.5 }, { 0,0,0 });
	hideBlock2_.Initialize({ -20,20,-40.5 }, { 0,0,0 });
	hideBlock3_.Initialize({ 20,20,-40.5 }, { 0,0,0 });

	textureHandle_ = TextureManager::Load("white1x1.png");
	UI = Sprite::Create(textureHandle_,{0,0});
	UI->SetSize({ 300,800 });
	UI->SetColor({ 0,0,0,1 });

	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GamePlayScene::Update()
{
	hideBlock1_.Update(Button::useCount,1);
	hideBlock2_.Update(Button::useCount,2);
	hideBlock3_.Update(Button::useCount,3);
	mouse->Update();
	blockManager->Update();
	player_.Update();
	skydome_->Update();
	// 当たり判定
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

#ifdef _DEBUG
	*viewProjection = debugCamera_->GetViewProjection(); 
#endif // _DEBUG
}

void GamePlayScene::Draw()
{
	// 3Dモデルの描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_.Draw();
	hideBlock1_.Draw();
	hideBlock2_.Draw();
	hideBlock3_.Draw();
	skydome_->Draw();
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	// スプライト描画
	Sprite::PreDraw(cmdList);

	UI->Draw();

	Sprite::PostDraw();
}