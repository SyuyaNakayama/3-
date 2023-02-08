#include "HideBlock.h"
#include <Input.h>
#include "GameScene.h"

void HideBlock::Initialize(Vector3 pos,)
{
	hideBlock = Model::CreateFromOBJ("Hblock");
	worldTransform_.Initialize();
	/*worldTransform_.scale_ = { 0.5f,0.511f,0.511f };*/
	worldTransform_.scale_ = { 20,20,1 };
	worldTransform_.translation_ = { pos.x,pos.y,pos.z };
    worldTransform_.rotation_ = { 0,0,0 };
}

void HideBlock::SetTransfer(Vector3 pos, Vector3 scale)
{
    worldTransform_.translation_ = pos;
    worldTransform_.scale_ = scale;
    worldTransform_.Update();
}

void HideBlock::Update(uint16_t useCount,uint16_t setNum)
{
    
    if (*GameScene::GetStage() == 1 || *GameScene::GetStage() == 3)//1‚©‚R
    {
        if (setNum == useCount)
        {
            worldTransform_.rotation_.z += 0.2f;
            worldTransform_.scale_.x -= 0.2f;
            worldTransform_.scale_.y -= 0.2f;
            //worldTransform.scale.z -= 0.01f;
            worldTransform_.Update();
        }

    }
    else
    {
        if (setNum == useCount)
        {
            worldTransform_.rotation_.x -= 0.2f;
            worldTransform_.scale_.y -= 0.2f;
            worldTransform_.scale_.z -= 0.2f;
            worldTransform_.Update();
        }

        /*if (worldTransform_.scale_.z <= 0)
        {
            isHide = false;
            worldTransform_.scale_.z = 20.0f;
            worldTransform_.scale_.y = 20.0f;
            worldTransform_.rotation_ = rot;
        }*/
    }
    
    

    if (worldTransform_.scale_.y <= 0)
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