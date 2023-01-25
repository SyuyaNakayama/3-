#include "TitleScene.h"
#include "TextureManager.h"
#include "GamePlayScene.h"
#include "GameScene.h"
#include "Quaternion.h"

void TitleScene::Initialize()
{
	gameScene = GameScene::GetInstance();
	viewProjection->target = { 40,0,40 };
	viewProjection->eye = { 0,1200,0 };
	viewProjection->up = upVector;
}

void TitleScene::Update()
{
	Quaternion rotaQ = { std::cos(cameraUpAngle / 2.0f) ,Vector3(0,1,0) * std::sin(cameraUpAngle / 2.0f) };
	viewProjection->up = Quaternion::RotateVector(Vector3(70, -40.0f, -1200), rotaQ);
	cameraUpAngle += 0.005f;
	if (input->TriggerKey(DIK_SPACE)) { gameScene->SetNextScene(Scene::Play); }
}

void TitleScene::Draw()
{
	Model::PreDraw(cmdList);
	blockManager->Draw();
	Model::PostDraw();
}