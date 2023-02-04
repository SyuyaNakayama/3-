#include "GamePlayScene.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "CollisionManager.h"
#include "Quaternion.h"
#include <imgui.h>

void GamePlayScene::Initialize()
{
	player_.Initialize(1);
	viewProjection->up = { 0,1,0 };
	viewProjection->eye = eyePos[0];
	viewProjection->target = targetPos[0];
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	preEyePos = eyePos[1];
	gameScene = GameScene::GetInstance();
	mouse->Initialize();
	blockManager->Initialize(1);
	Button::SetUseCount(1);
}

void GamePlayScene::Update()
{
	// �X�e�[�W�N���A��
	if (GoalBlock::IsGoal())
	{
		if (ChangeNextStage()) { return; } // �J������Ԓ�
		GoalBlock::SetIsGoal(false); // �J������ԏI��
		t = 0;
		player_.Initialize(stage);
	}
	// �J�������[�N
	if (CameraLerp()) { return; }

	mouse->Update();
	blockManager->Update();
	player_.Update();

	// �����蔻��
	CollisionManager::CheckAllCollisions();

	debugCamera_->Update();

#ifdef _DEBUG
	* viewProjection = debugCamera_->GetViewProjection();
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
	if (!(Button::GetUseCount() == 1 && !isCameraLerp)) { return false; }
	t += dt;
	if (t > 1.0f) { t = 1.0f; }
	Quaternion rotQ = CubeQuaternion::Get(stage);
	Vector3 eye_lerp[2]{}, target_lerp[2]{};
	for (size_t i = 0; i < 2; i++)
	{
		eye_lerp[i] = RotateVector(eyePos[i], rotQ);
		target_lerp[i] = RotateVector(targetPos[i], rotQ);
	}

	auto lerpFunc = [&](size_t startIndex, size_t endIndex)
	{
		viewProjection->eye = lerp(RotateVector(eye_lerp[startIndex], rotQ), RotateVector(eye_lerp[endIndex], rotQ), t);
		viewProjection->target = lerp(RotateVector(target_lerp[startIndex], rotQ), RotateVector(target_lerp[endIndex], rotQ), t);
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
		blockManager->Initialize(++stage);
		blockManager->Update();
	}
	t += dt;
	if (t > 1.0f) { t = 1.0f; }
	UINT16 nowStage = stage;
	if (nowStage >= 6) { nowStage = 0; }
	Quaternion rotNextQ =
		Slerp(Quaternion::Normalize(CubeQuaternion::Get(stage - 1)), Quaternion::Normalize(CubeQuaternion::Get(nowStage)), t);
	viewProjection->eye = RotateVector(preEyePos, rotNextQ);
	return t < 1.0f;
}