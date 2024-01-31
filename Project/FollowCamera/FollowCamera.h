#pragma once
#include "3D/Matrix/ViewProjection.h"
#include "3D/Matrix/WorldTransform.h"

class FollowCamera {
public:
	/// 
	/// Default Method
	/// 

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	/// 
	/// User Method
	/// 

	//void GoalCamera

	/// Getter
	/// ビュープロジェクションを取得
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	/// Setter
	// 追従する対象を設定
	void SetTarget(const WorldTransform* target) { target_ = target; }

	// 追従対象とカメラの距離の設定
	void SetCameraToTargetDist(Vector3 offset) { offset_ = offset; }

private:// プライベートな関数
	// ゴール時のカメラの角度を設定
	void GoalAngle();

	// ゴール時の追従対象とカメラの距離を設定
	void GoalCameraToTargetDist();

	// イージング
	float EaseOutExpo(float x);

	// 回転行列の作成
	Matrix4x4 MakeRotateMatrix(const Vector3& radian);

private:// 定数
	// イージング終了までにかかるフレーム
	const int kEndEaseFrame = 35;

	// イージングする回数
	const int kMaxEase = 3;

private:
	ViewProjection viewProjection_;
	const WorldTransform* target_ = nullptr;
	// 追従対象からカメラまでのオフセット
	Vector3 offset_;

	// ゴールした時のカメラ移動フラグ
	bool isGoalAngle_;

	bool isGoalMove_;

	// イージングフレーム
	float easeFrame_;
	// イージングの進行度(1になったら終了)
	float t_;
	// イージングした回数
	int easeCount_;
};