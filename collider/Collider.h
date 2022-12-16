#pragma once
#include "WorldTransform.h"
#include <cstdint>

class Collider
{
private:
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t collisionMask_ = 0xffffffff;

protected:
	WorldTransform worldTransform;

public:
	virtual void OnCollision() {};
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); };
	virtual Vector3 GetRadius() { return worldTransform.scale_; };

	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	uint32_t GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }
};