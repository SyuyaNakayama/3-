#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Mouse.h"

class GamePlayScene : public BaseScene
{
private:
	Player player_;
	std::unique_ptr<DebugCamera> debugCamera_;
	Mouse* mouse = Mouse::GetInstance();
	Vector3 eyePos[2] = { { 20,-20,-600 },{ 0,0,-1200 } };
	Vector3 preEyePos{}; // �J������ԑO��eye�̏ꏊ
	Vector3 targetPos[2] = { { 20,-20,0 },{ 0,0,0 } };
	bool isCameraLerp = false;
	float t = 0; // ��ԂɎg���ϐ�
	float dt = 0.01f; // t�̑�����
	UINT16* stage = nullptr;

public:
	void Initialize() override;
	void Finalize() {};
	void Update() override;
	void Draw() override;
	// �J�������(�J�����ړ�����true��Ԃ�)
	bool CameraLerp(bool isFlip = false); // ������L����(���߂�)���
	bool ChangeNextStage(); // ���̃X�e�[�W�ɑJ�ڂ�����
};