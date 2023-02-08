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

void HideBlock::Update(uint16_t useCount,uint16_t setNum)
{
    input = Input::GetInstance();
    //if (input->PushKey(DIK_1))
    //{
    //    worldTransform_.rotation_.z += 0.2f;
    //    worldTransform_.scale_.x -= 0.2f;
    //    worldTransform_.scale_.y -= 0.2f;
    //    //worldTransform.scale.z -= 0.01f;
    //    worldTransform_.Update();
    //}

    if (setNum == useCount)
    {
        worldTransform_.rotation_.z += 0.2f;
        worldTransform_.scale_.x -= 0.2f;
        worldTransform_.scale_.y -= 0.2f;
        //worldTransform.scale.z -= 0.01f;
        worldTransform_.Update();
    }

    if (worldTransform_.scale_.x <= 0)
    {
        isHide = false;
    }

    worldTransform_.Update();
}

void HideBlock::Draw()
{
    if(isHide == true){
        hideBlock->Draw(worldTransform_, *ViewProjection::GetInstance());
    }
}