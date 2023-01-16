#pragma once
#include "Input.h"
#include "Matrix4.h"
#include "ViewProjection.h"
#include "MathUtility.h"
#include "Collider.h"

class Mouse final : public RayCollider
{
private:
	Input* input = Input::GetInstance();
	Vector2 mousePos{};
	Matrix4 mat = MathUtility::Matrix4Identity();
	Matrix4 matViewPort = MathUtility::Matrix4Identity();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Vector3 posNear{};

	~Mouse() = default;
public:
	static Mouse* GetInstance();

	void Update();
	// ビューポート行列の生成
	void Initialize();
	Vector2 GetMousePos() { return mousePos; }
	Matrix4 GetMatrix() { return mat; }
	Vector3 GetNearPos() { return posNear; }
	Vector3 GetWorldPosition() { return GetNearPos(); }
};