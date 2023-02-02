#pragma once
#include "Input.h"
#include "Matrix4.h"
#include "ViewProjection.h"
#include "MathUtility.h"
#include "Collider.h"

class Mouse final : public RayCollider
{
private:
	Vector3 posNear{};
	Matrix4 matViewPort = MathUtility::Matrix4Identity();

	~Mouse() = default;
public:
	static Mouse* GetInstance();

	void Update();
	// ビューポート行列の生成
	void Initialize();
	Vector3 GetWorldPosition() { return posNear; }
};