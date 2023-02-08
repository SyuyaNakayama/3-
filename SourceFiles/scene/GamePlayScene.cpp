#include "GamePlayScene.h"
#include "GameScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include "WinApp.h"
#include "ViewProjection.h"
#include <imgui.h>

void GamePlayScene::Initialize()
{
	stage = GameScene::GetStage();
	player_.Initialize();
	viewProjection->up = { 0,1,0 };
	viewProjection->eye = RotateVector(eyePos[0], CubeQuaternion::Get());
	viewProjection->target = RotateVector(targetPos[0], CubeQuaternion::Get());
	gameScene = GameScene::GetInstance();
	mouse->Initialize();
	blockManager->Initialize();
	GoalBlock::SetIsGoal(false);
	debugCamera = std::make_unique<DebugCamera>(1280, 720);
	
	Button::SetUseCount(0);

	textureHandle_ = TextureManager::Load("UI/UIBack.png");
	UI = Sprite::Create(textureHandle_, { 0,0 });
	UI->SetSize({ 285,720 });
	UI->SetPosition({ 995,0 });
	//ステージ数
	{
		textureHandleTuto_ = TextureManager::Load("UI/tutorial.png");
		UITuto = Sprite::Create(textureHandleTuto_, { 0,0 });
		UITuto->SetSize({ 285,100 });
		UITuto->SetPosition({ 995,0 });

		textureHandleStage1_ = TextureManager::Load("UI/stage1.png");
		UIStage1 = Sprite::Create(textureHandleStage1_, { 0,0 });
		UIStage1->SetSize({ 285,100 });
		UIStage1->SetPosition({ 995,0 });

		textureHandleStage2_ = TextureManager::Load("UI/stage2.png");
		UIStage2 = Sprite::Create(textureHandleStage2_, { 0,0 });
		UIStage2->SetSize({ 285,100 });
		UIStage2->SetPosition({ 995,0 });

		textureHandleStage3_ = TextureManager::Load("UI/stage3.png");
		UIStage3 = Sprite::Create(textureHandleStage3_, { 0,0 });
		UIStage3->SetSize({ 285,100 });
		UIStage3->SetPosition({ 995,0 });
	}
	//イラスト説明
	{
		textureHandleDdMark_ = TextureManager::Load("UI/ddMark.png");
		UIDdMark = Sprite::Create(textureHandleDdMark_, { 0,0 });
		UIDdMark->SetSize({ 285,150 });
		UIDdMark->SetPosition({ 995,100 });

		textureHandleButtonMark_ = TextureManager::Load("UI/buttonMark.png");
		UIButtonMark = Sprite::Create(textureHandleButtonMark_, { 0,0 });
		UIButtonMark->SetSize({ 285,150 });
		UIButtonMark->SetPosition({ 995,100 });

		textureHandleLadderMark_ = TextureManager::Load("UI/ladderMark.png");
		UILadderMark = Sprite::Create(textureHandleLadderMark_, { 0,0 });
		UILadderMark->SetSize({ 285,150 });
		UILadderMark->SetPosition({ 995,100 });

		textureHandleDestroyMark_ = TextureManager::Load("UI/destroyMark.png");
		UIDestroyMark = Sprite::Create(textureHandleDestroyMark_, { 0,0 });
		UIDestroyMark->SetSize({ 285,150 });
		UIDestroyMark->SetPosition({ 995,100 });

		textureHandleCopyMark_ = TextureManager::Load("UI/copyMark.png");
		UICopyMark = Sprite::Create(textureHandleCopyMark_, { 0,0 });
		UICopyMark->SetSize({ 285,150 });
		UICopyMark->SetPosition({ 995,100 });

		textureHandleGoalMark_ = TextureManager::Load("UI/goalMark.png");
		UIGoalMark = Sprite::Create(textureHandleGoalMark_, { 0,0 });
		UIGoalMark->SetSize({ 285,150 });
		UIGoalMark->SetPosition({ 995,100 });
	}
	//テキスト説明
	{

		textureHandleDdText_ = TextureManager::Load("UI/ddText.png");
		UIDdText = Sprite::Create(textureHandleDdText_, { 0,0 });
		UIDdText->SetSize({ 285,400 });
		UIDdText->SetPosition({ 995,250 });

		textureHandleButtonText_ = TextureManager::Load("UI/buttonText.png");
		UIButtonText = Sprite::Create(textureHandleButtonText_, { 0,0 });
		UIButtonText->SetSize({ 285,400 });
		UIButtonText->SetPosition({ 995,250 });

		textureHandleLadderText_ = TextureManager::Load("UI/ladderText.png");
		UILadderText = Sprite::Create(textureHandleLadderText_, { 0,0 });
		UILadderText->SetSize({ 285,400 });
		UILadderText->SetPosition({ 995,250 });

		textureHandleDestroyText_ = TextureManager::Load("UI/destroyText.png");
		UIDestroyText = Sprite::Create(textureHandleDestroyText_, { 0,0 });
		UIDestroyText->SetSize({ 285,400 });
		UIDestroyText->SetPosition({ 995,250 });

		textureHandleCopyText_ = TextureManager::Load("UI/copyText.png");
		UICopyText = Sprite::Create(textureHandleCopyText_, { 0,0 });
		UICopyText->SetSize({ 285,400 });
		UICopyText->SetPosition({ 995,250 });

		textureHandleGoalText_ = TextureManager::Load("UI/goalText.png");
		UIGoalText = Sprite::Create(textureHandleGoalText_, { 0,0 });
		UIGoalText->SetSize({ 285,400 });
		UIGoalText->SetPosition({ 995,250 });
	}

	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R)) { gameScene->SetNextScene(Scene::Play); }
	if (input->TriggerKey(DIK_T)) { gameScene->SetNextScene(Scene::Title); }

	// ステージクリア時
	if (GoalBlock::IsGoal())
	{
		
		if (*stage <= 3 || isCameraScroll)
		{
			if (isCameraLerp)
			{
				isCameraScroll = true;
				if (ChangeNextStage()) { return; } // カメラ補間中

				t = 0; isCameraLerp = false;
			}
			// カメラズームイン
			if (!isCameraLerp)
			{
				if (CameraLerp(true)) { return; }
				player_.Initialize();
				GoalBlock::SetIsGoal(false); // カメラ補間終了
				isCameraLerp = false;
				isCameraScroll = false;
				Button::SetUseCount(0);
			}
		}
		else { gameScene->SetNextScene(Scene::Clear); }
	}
	// カメラズームアウト
	if (Button::GetUseCount() >= 1 && !isCameraLerp) { if (CameraLerp()) { return; } }


	mouse->Update();
	blockManager->Update();
	player_.Update();
	skydome_->Update();
	// 当たり判定
	CollisionManager::CheckAllCollisions();
	debugCamera->Update();
	//*viewProjection = debugCamera->GetViewProjection();
}

void GamePlayScene::Draw()
{
	Sprite::PreDraw(cmdList);
	blockManager->SpriteDraw();
	Sprite::PostDraw();
	// 3Dモデルの描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(cmdList);

	blockManager->Draw();
	
	player_.Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	// スプライト描画
	Sprite::PreDraw(cmdList);
	
	UITuto->Draw();
	UIDdMark->Draw();
	UIDdText->Draw();

	Sprite::PostDraw();
}

bool GamePlayScene::CameraLerp(bool isFlip)
{
	t += dt;
	if (t > 1.0f) { t = 1.0f; }
	Quaternion rotQ = CubeQuaternion::Get();
	Vector3 eye_lerp[2]{}, target_lerp[2]{};
	for (size_t i = 0; i < 2; i++)
	{
		eye_lerp[i] = RotateVector(eyePos[i], rotQ);
		target_lerp[i] = RotateVector(targetPos[i], rotQ);
	}

	auto lerpFunc = [&](size_t startIndex, size_t endIndex)
	{
		viewProjection->eye = lerp(eye_lerp[startIndex], eye_lerp[endIndex], t);
		viewProjection->target = lerp(target_lerp[startIndex], target_lerp[endIndex], t);
	};
	if (!isFlip) { lerpFunc(0, 1); }
	else { lerpFunc(1, 0); }
	if (t >= 1.0f) { t = 0; isCameraLerp = true; return false; }
	return true;
}

bool GamePlayScene::ChangeNextStage()
{
	if (t == 0.0f)
	{
		blockManager->Clear();
		++* stage;
		blockManager->Initialize();
		blockManager->Update();
	}
	t += dt;
	if (t > 1.0f) { t = 1.0f; }
	Quaternion rotNextQ =
		Slerp(Quaternion::Normalize(CubeQuaternion::Get(*stage - 1)), Quaternion::Normalize(CubeQuaternion::Get(*stage)), t);
	viewProjection->eye = RotateVector(eyePos[1], rotNextQ);
	return t < 1.0f;
}