#pragma once
#include "WorldTransform.h"
#include <array>

enum class CollisionAttribute
{
	Player = 0b1,
	Block = 0b1 << 1,
	All = -1
};

enum class CollisionMask
{
	Player = ~(int)CollisionAttribute::Player,
	Block = ~(int)CollisionAttribute::Block,
	All = -1
};

class BoxCollider;
class SphereCollider;
class PlaneCollider;
class TriangleCollider;
class RayCollider;

class BaseCollider
{
private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::All;
	CollisionMask collisionMask_ = CollisionMask::All;

protected:
	WorldTransform worldTransform;

public:
	~BaseCollider() = default;

	virtual void OnCollision(BoxCollider* boxCollider) {}
	virtual void OnCollision(SphereCollider* sphereCollider) {}
	virtual void OnCollision(PlaneCollider* boxCollider) {}
	virtual void OnCollision(TriangleCollider* sphereCollider) {}
	virtual void OnCollision(RayCollider* sphereCollider) {}
	virtual Vector3 GetWorldPosition() = 0;

	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }
	CollisionMask GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(CollisionMask collisionMask) { collisionMask_ = collisionMask; }
};

class BoxCollider : public BaseCollider
{
public:
	BoxCollider();
	~BoxCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRadius() { return worldTransform.scale_; }
};

class SphereCollider : public BaseCollider
{
public:
	SphereCollider();
	~SphereCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual float GetRadius() { return worldTransform.scale_.x; }
};

class PlaneCollider : public BaseCollider
{
protected:
	Vector3 normal{};
	float distance;

public:
	PlaneCollider();
	~PlaneCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return normal; }
	virtual float GetDistance() { return distance; }
};

class TriangleCollider : public PlaneCollider
{
protected:
	Vector3 normal{};
	std::array<Vector3, 3> vertices;

public:
	TriangleCollider();
	~TriangleCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return normal; }
	virtual std::array<Vector3, 3> GetVertices() { return vertices; }
};

class RayCollider : public BaseCollider
{
protected:
	Vector3 direction{};

public:
	RayCollider();
	~RayCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRayDirection() { return direction; }
};