#include "HideBlock.h"
#include <Input.h>
#include "GameScene.h"
#include <imgui.h>
#include "Block.h"

void HideBlock::Initialize()
{
	hideBlock = Model::CreateFromOBJ("Hblock");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 20,20,1 };
    isHide = true;
}

void HideBlock::SetTransfer(Vector3 pos, Vector3 scale)
{
    worldTransform_.translation_ = pos;
    worldTransform_.scale_ = scale;
    worldTransform_.Update();
}

void HideBlock::Update(uint16_t useCount,uint16_t setNum)
{
    
    if (*GameScene::GetStage() == 1 || *GameScene::GetStage() == 3)//1か３
    {
        if (setNum == useCount)
        {
            worldTransform_.rotation_.z += 0.2f;
            worldTransform_.scale_.x -= 0.2f;
            worldTransform_.scale_.y -= 0.2f;
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
    }
    
    if (worldTransform_.scale_.y <= 0)
    {
        isHide = false;
    }
 
    if(GoalBlock::IsGoal() == true) //次のステージへ行く時に角度を直す
    {
        worldTransform_.rotation_ = {0,0,0}; 
    }
    
    worldTransform_.Update();
}

void HideBlock::Draw()
{
    if(isHide == true){
        hideBlock->Draw(worldTransform_, *ViewProjection::GetInstance());
    }
}