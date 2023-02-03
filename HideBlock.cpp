#include "HideBlock.h"
#include <Input.h>

void HideBlock::Initialize()
{
	hideBlock = Model::CreateFromOBJ("hideblock");

	worldTransform_.Initialize();
	

	worldTransform_.scale_ = { 0.5f,0.511f,0.511f };
	worldTransform_.rotation_ = { 0,1.555f,0 };
	worldTransform_.translation_ = { 19.0f,-19.0f,-1.0f };
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
}

void HideBlock::Draw()
{
	hideBlock->Draw(worldTransform_, *ViewProjection::GetInstance());
}