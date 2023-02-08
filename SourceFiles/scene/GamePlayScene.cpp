#include "GamePlayScene.h"
#include "GameScene.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include "WinApp.h"
#include "ViewProjection.h"

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

	hideBlock1_.Initialize({ -20,-20,-40.5 }, { 0,0,0 });
	hideBlock2_.Initialize({ -20,20,-40.5 }, { 0,0,0 });
	hideBlock3_.Initialize({ 20,20,-40.5 }, { 0,0,0 });

	textureHandle_ = TextureManager::Load("white1x1.png");

	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GamePlayScene::Update()
{
	if (input->TriggerKey(DIK_R)) { gameScene->SetNextScene(Scene::Play); }
	if (input->TriggerKey(DIK_T)) { gameScene->SetNextScene(Scene::Title); }

	// �X�e�[�W�N���A��
	if (GoalBlock::IsGoal())
	{
		if (*stage <= 3 || isCameraScroll)
		{
			if (isCameraLerp)
			{
				isCameraScroll = true;
				if (ChangeNextStage()) { return; } // �J������Ԓ�
				t = 0; isCameraLerp = false;
			}
			// �J�����Y�[���C��
			if (!isCameraLerp)
			{
				if (CameraLerp(true)) { return; }
				player_.Initialize();
				GoalBlock::SetIsGoal(false); // �J������ԏI��
				isCameraLerp = false;
				isCameraScroll = false;
			}
		}
		else { gameScene->SetNextScene(Scene::Clear); }
	}
	// �J�����Y�[���A�E�g
	if (Button::GetUseCount() >= 1 && !isCameraLerp) { if (CameraLerp()) { return; } }

	hideBlock1_.Update(Button::GetUseCount(), 1);
	hideBlock2_.Update(Button::GetUseCount(), 2);
	hideBlock3_.Update(Button::GetUseCount(), 3);
	mouse->Update();
	blockManager->Update();
	player_.Update();
	skydome_->Update();
	// �����蔻��
	CollisionManager::CheckAllCollisions();
	debugCamera->Update();
	//*viewProjection = debugCamera->GetViewProjection();
}

void GamePlayScene::Draw()
{
	Sprite::PreDraw(cmdList);
	blockManager->SpriteDraw();
	Sprite::PostDraw();
	// 3D���f���̕`��
	// 3D�I�u�W�F�N�g�`��O����
	Model::PreDraw(cmdList);

	blockManager->Draw();
	player_.Draw();
	hideBlock1_.Draw();
	hideBlock2_.Draw();
	hideBlock3_.Draw();
	skydome_->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	Model::PostDraw();
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