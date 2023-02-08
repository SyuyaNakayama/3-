#include "ClearScene.h"
#include "GameScene.h"
#include "Quaternion.h"

void ClearScene::Initialize()
{
	*GameScene::GetStage() = 5;
	gameScene = GameScene::GetInstance();
	viewProjection->target = {};
	viewProjection->eye = { 0,-1200,0 };
	viewProjection->up = upVector;
	BlockManager::GetInstance()->Initialize();
	sprite.reset(Sprite::Create(TextureManager::Load("UI/key_SPACE.png"), { 640,640 }, { 1,1,1,1 }, { 0.5f,0.5f }));
	sprite->SetSize({ 256,128 });
}

void ClearScene::Update()
{
	Quaternion rotaQ = { std::cos(cameraUpAngle / 2.0f) ,Vector3(0,1,0) * std::sin(cameraUpAngle / 2.0f) };
	viewProjection->up = RotateVector(Vector3(-1, 0, 0), rotaQ);
	cameraUpAngle += upRotSpd;
	
	if (animeTimer.CountDown())
	{
		animeFrame++;
		if (animeFrame >= 2) { animeFrame = 0; }
	}
	sprite->SetTextureRect({ (float)animeFrame * 128,0 }, { 128,64 });
	
	if (input->TriggerKey(DIK_SPACE))
	{
		isNextScene = true;
		upRotSpd = 0.075f;
	}
	if (!isNextScene) { return; }
	if (timer.CountDown()) { gameScene->SetNextScene(Scene::Title); }
}

void ClearScene::Draw()
{
	Model::PreDraw(cmdList);
	blockManager->Draw();
	Model::PostDraw();

	Sprite::PreDraw(cmdList);
	sprite->Draw();
	Sprite::PostDraw();
}