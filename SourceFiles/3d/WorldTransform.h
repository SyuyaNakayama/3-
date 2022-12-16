#pragma once

#include "Vector3.h"
#include "Matrix4.h"
#include <d3d12.h>
#include <wrl.h>
#include "MathUtility.h"

// 定数バッファ用データ構造体
struct ConstBufferDataWorldTransform {
	Matrix4 matWorld;           // ローカル → ワールド変換行列
};
	
using namespace MathUtility;

/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 translation_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();

	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void Update() {
		Matrix4 matScale = Matrix4Scaling(scale_.x, scale_.y, scale_.z),

			matRot = Matrix4RotationX(rotation_.x) *
			Matrix4RotationY(rotation_.y) *
			Matrix4RotationZ(rotation_.z),

			matTrans = Matrix4Translation(translation_.x, translation_.y, translation_.z);

		matWorld_ = matScale * matRot * matTrans;

		if (parent_) { matWorld_ *= parent_->matWorld_; }

		TransferMatrix();
	}

	Vector3 GetWorldPosition() { return { matWorld_.m[3][0], matWorld_.m[3][1], matWorld_.m[3][2] }; }
};