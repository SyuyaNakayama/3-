#include "SoundManager.h"

void SoundManager::Initialize()
{
	audio_ = Audio::GetInstance();
	playHandle_.resize(bgm_.size());
}

void SoundManager::PlayBGM(BGM index)
{
	if (audio_->IsPlaying(playHandle_[index])) { return; }
	playHandle_[index] = audio_->PlayWave(bgm_[index], true, 0.8f);
}

void SoundManager::PlaySE(SE index)
{
	 audio_->PlayWave(se_[index]);
}

void SoundManager::StopBGM(BGM index)
{
 	audio_->StopWave(playHandle_[index]);
}

SoundManager* SoundManager::GetInstance()
{
	static SoundManager instance;
	return &instance;
}