#pragma once
#include "WorldTransform.h"
#include <vector>

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
class PolygonCollider;
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
	virtual void OnCollision(PolygonCollider* sphereCollider) {}
	virtual void OnCollision(RayCollider* sphereCollider) {}

	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }
	CollisionMask GetCollisionMask() { return collisionMask_; }
	void SetCollisionAttribute(CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }
	void SetCollisionMask(CollisionMask collisionMask) { collisionMask_ = collisionMask; }
};

class BoxCollider : public virtual BaseCollider
{
public:
	BoxCollider();
	~BoxCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRadius() { return worldTransform.scale_; }
};

class SphereCollider : public virtual BaseCollider
{
public:
	SphereCollider();
	~SphereCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual float GetRadius() { return worldTransform.scale_.x; }
};

class PlaneCollider : public virtual BaseCollider
{
protected:
	Vector3 normal{};
	float distance = 0;

public:
	PlaneCollider();
	~PlaneCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return normal; }
	virtual float GetDistance() { return distance; }
};

class PolygonCollider : public virtual PlaneCollider
{
protected:
	std::vector<Vector3> vertices;

public:
	PolygonCollider();
	~PolygonCollider();

	void ComputeDistance() { distance = normal.dot(vertices[0]); }
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetNormal() { return normal; }
	virtual void SetVertices();
	virtual std::vector<Vector3> GetVertices() { return vertices; }
};

class RayCollider : public virtual BaseCollider
{
protected:
	Vector3 direction{};

public:
	RayCollider();
	~RayCollider();

	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
	virtual Vector3 GetRayDirection() { return direction; }
};