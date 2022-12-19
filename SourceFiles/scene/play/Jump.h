#pragma once
class Jump
{
private:
	bool isJump_ = false;
	float jumpSpd_ = 0;
	float gravity_ = 0;
	float endHeight_ = 0;
	bool isFall = false;

public:
	void SetGravity(float gravity) { gravity_ = gravity; }
	void StartJump(float jumpV0, float endHeight);
	void UpdateJump(float& y);
	void StartFall();
	void UpdateFall(float& y);
	void EndFall() { isFall = false; }
	bool IsJump() { return isJump_; jumpSpd_ = 0; }
	bool IsFall() { return isFall; }
};