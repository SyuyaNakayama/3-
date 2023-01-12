#pragma once
#include "Input.h"
#include "Matrix4.h"
#include "ViewProjection.h"
#include "MathUtility.h"

class Mouse final
{
private:
	Input* input = Input::GetInstance();
	Vector2 mousePos{};
	Vector3 mouseDirection;
	Matrix4 mat = MathUtility::Matrix4Identity();
	Matrix4 matViewPort = MathUtility::Matrix4Identity();
	ViewProjection* viewProjection = ViewProjection::GetInstance();
	Vector3 posNear{};

	~Mouse() = default;
public:
	static Mouse* GetInstance();

	void Update();
	// ビューポート行列の生成
	void CreateViewPortMatrix();
	Vector2 GetMousePos() { return mousePos; }
	Vector3 GetMouseDirection() { return mouseDirection; }
	Matrix4 GetMatrix() { return mat; }
	Vector3 GetNearPos() { return posNear; }
};