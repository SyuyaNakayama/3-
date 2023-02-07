#include "HideBlock.h"
#include <Input.h>

void HideBlock::Initialize(Vector3 pos, Vector3 rot)
{
	hideBlock = Model::CreateFromOBJ("Hblock");
	worldTransform_.Initialize();
	/*worldTransform_.scale_ = { 0.5f,0.511f,0.511f };*/
	worldTransform_.scale_ = { 20,20,1 };
	worldTransform_.translation_ = { pos.x,pos.y,pos.z };
	worldTransform_.rotation_ = { rot.x,rot.y,rot.z };
	
}

void HideBlock::Update()
{
	input = Input::GetInstance();
	if (input->PushKey(DIK_1))
	{
		worldTransform_.rotation_.x += 0.2f;
		worldTransform_.scale_.x -= 0.01f;
		worldTransform_.scale_.y -= 0.01f;
		worldTransform_.scale_.z -= 0.01f;
		worldTransform_.Update();
	}
	worldTransform_.Update();
}

void HideBlock::Draw()
{
	hideBlock->Draw(worldTransform_, *ViewProjection::GetInstance());
}