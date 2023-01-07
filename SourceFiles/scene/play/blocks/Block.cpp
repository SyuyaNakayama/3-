#include "Block.h"
#include "ViewProjection.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "WinApp.h"

void Block::DragBox()
{
	// �C���X�^���X�̎擾
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Input* input = Input::GetInstance();
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// block�̍��W���擾
	Vector3 blockPos = worldTransform.GetWorldPosition();
	// �r���[�|�[�g�s��̐���
	Matrix4 matViewPort = Matrix4Identity();
	matViewPort.m[0][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[1][1] = -(float)WinApp::kWindowHeight / 2;
	matViewPort.m[3][0] = (float)WinApp::kWindowWidth / 2;
	matViewPort.m[3][1] = (float)WinApp::kWindowHeight / 2;
	// �r���[�A�v���W�F�N�V�����A�r���[�|�[�g�s��̊|���Z
	Matrix4 mat = viewProjection->matView * viewProjection->matProjection * matViewPort;
	// block��2D���W�A���a���v�Z
	Vector3 block2DRad = Vector3TransformCoord(blockPos - worldTransform.scale_, mat);
	Vector3 block2DPos = Vector3TransformCoord(blockPos, mat);
	// �}�E�X���W���擾
	Vector2 mousePos = input->GetMousePosition();
	// �}�E�X���W��block��2D���W�������ɂ�������
	bool flag = isDrag || (fabs(block2DPos.x - (float)mousePos.x) <= fabs(block2DPos.x - block2DRad.x) &&
		fabs(block2DPos.y - (float)mousePos.y) <= fabs(block2DPos.y - block2DRad.y));
	if (!flag) { return; }

	// �}�E�X�̍��{�^���������ꂽ�ꍇ
	if (input->IsTriggerMouse(0)) { isDrag = true; }
	if (!isDrag) { return; }

	// �X�N���[�����W
	Vector3 posNear = { mousePos.x,mousePos.y,0 };
	Vector3 posFar = { mousePos.x,mousePos.y,1 };
	// �X�N���[�����W�����[���h���W�ɕϊ�
	posNear = Vector3TransformCoord(posNear, Matrix4Inverse(mat));
	posFar = Vector3TransformCoord(posFar, Matrix4Inverse(mat));
	// �}�E�X���C�̕���
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	// �J��������block�̋���
	float distanceObject = Vector3Length(viewProjection->eye - blockPos);
	worldTransform.translation_ = posNear + mouseDirection * distanceObject;
	worldTransform.translation_.z = 0;
	if (!input->IsPressMouse(0)) { isDrag = false; }
}

void Block::Initialize()
{
	worldTransform.Initialize();
	model = Model::Create();
	SetCollisionAttribute(CollisionAttribute::Block);
	SetCollisionMask(CollisionMask::Block);
}

void Block::Update()
{
	DragBox();
	worldTransform.Update();
}

void Block::Draw()
{
	model->Draw(worldTransform, *ViewProjection::GetInstance());
}