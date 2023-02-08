#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include <imgui.h>

void GamePlayScene::Initialize()
{
	stage = GameScene::GetStage();
	player_.SetStage(stage);
	player_.Initialize();
	viewProjection->up = { 0,1,0 };
	viewProjection->eye = RotateVector(eyePos[0],CubeQuaternion::Get());
	viewProjection->target = RotateVector(targetPos[0], CubeQuaternion::Get());
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	preEyePos = eyePos[1];
	gameScene = GameScene::GetInstance();
	mouse->Initialize();
	BaseBlock::SetStage(stage);
	StagePlane::GetInstance()->SetStage(stage);
	blockManager->Initialize(*stage);
	Button::SetUseCount(0);
}

void GamePlayScene::Update()
{
	imguiManager->PrintVector("stagePlaneInter", *StagePlane::GetInstance()->GetInter());
	imguiManager->PrintVector("stagePlaneNormal", StagePlane::GetInstance()->GetNormal());
	if(input->TriggerKey(DIK_R))
	{
		gameScene->SetNextScene(Scene::Play);
	}
	if(input->TriggerKey(DIK_T))
	{
		stage = 0;
		gameScene->SetNextScene(Scene::Title);
	}

	// �X�e�[�W�N���A��
	if (GoalBlock::IsGoal())
	{
		if (isCameraLerp)
		{
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
			Button::SetUseCount(0);
			StagePlane* p = StagePlane::GetInstance();
		}
	}
	// �J�����Y�[���A�E�g
	if (Button::GetUseCount() >= 1 && !isCameraLerp) { if (CameraLerp()) { return; } }

	mouse->Update();
	blockManager->Update();
	player_.Update();

	// �����蔻��
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

#ifdef _DEBUG
	//*viewProjection = debugCamera_->GetViewProjection();
#endif // _DEBUG
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
		blockManager->Initialize(++*stage);
		blockManager->Update();
	}
	t += dt;
	if (t > 1.0f) { t = 1.0f; }
	Quaternion rotNextQ =
		Slerp(Quaternion::Normalize(CubeQuaternion::Get(*stage - 1)), Quaternion::Normalize(CubeQuaternion::Get(*stage)), t);
	viewProjection->eye = RotateVector(preEyePos, rotNextQ);
	return t < 1.0f;
}