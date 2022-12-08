#include "AnimationManager.h"

void AnimationManager::SetSprite(uint32_t textureHandle, uint32_t frameNum, int frameTime, Vector2 textureSize)
{
	std::vector<Sprite*> spriteArray;
	for (uint32_t i = 0; i < frameNum; i++)
	{
		spriteArray.push_back(Sprite::Create(textureHandle, {}));
		spriteArray[i]->SetTextureRect({ textureSize.x * (float)i,0 }, { textureSize.x * frameNum,textureSize.y });
	}
	sprite_.push_back(spriteArray);
	timer_.push_back(frameTime);
	frame_.push_back(0);
}

void AnimationManager::Update(int index)
{
	if (!timer_[index].CountDown()) { return; }
	frame_[index]++;
	if (frame_[index] != sprite_[index].size()) { return; }
	frame_[index] = 0;
}

void AnimationManager::Draw(int index)
{
	sprite_[index][frame_[index]]->Draw();
}