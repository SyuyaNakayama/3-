#include "Mouse.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "Collision.h"
#include <imgui.h>
using namespace MathUtility;

Mouse* Mouse::GetInstance()
{
	static Mouse instance;
	return &instance;
}

void Mouse::Update()
{
	mousePos = input->GetMousePosition();

	// �X�N���[�����W
	posNear = { mousePos.x,mousePos.y,0 };
	Vector3 posFar = { mousePos.x,mousePos.y,1 };

	// �r���[�A�v���W�F�N�V�����A�r���[�|�[�g�s��̊|���Z
	mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// �X�N���[�����W�����[���h���W�ɕϊ�
	posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
	posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));

	// �}�E�X���C�̕���
	direction = posFar - posNear;
	direction.normalize();
}

void Mouse::Initialize()
{
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;

	SetCollisionAttribute(CollisionAttribute::MouseRay);
	SetCollisionMask(CollisionMask::MouseRay);
}