#include "FollowCamera.h"
#include "Utility/MathFunction.h"
#include "2D/ImGuiManager.h"
#include "Utility/MathFunction.h"
#include <Xinput.h>

void FollowCamera::Initialize() {
	viewProjection_.Initialize();
	// 追従対象からカメラまでのオフセット
	offset_ = { 0.0f, 8.0f, -65.0f };

	// ゴールした時のカメラ移動フラグ
	isGoalAngle_ = false;
	isGoalMove_ = false;
	isStartCamera_ = false;

	t_ = 0;
	easeCount_ = 0;
}

void FollowCamera::Update() {
	if (nextTarget_) {
		Vector3 offset = nextTarget_->translation_;
		offset.z -= 20;
		vel_ = Multiply(0.2f, Subtract(offset, viewProjection_.translation_));
		viewProjection_.translation_ = Add(viewProjection_.translation_, vel_);
		if (offset.y == viewProjection_.translation_.y) {
			isStartCamera_ = false;
		}
	}
	else {
		if (target_) {
			Vector3 offset = offset_;
			// カメラの角度から回転行列を計算
			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

			// オフセットをカメラの回転に合わせて回転
			offset = TransformNormal(offset, rotateMatrix);

			// 座標をコピーしてオフセット分ずらす。ただしx座標はずらさない
			viewProjection_.translation_ = Add(target_->translation_, offset);
			viewProjection_.translation_.z -= viewProjection_.translation_.y / 2;
			viewProjection_.translation_.y = 6.0f;
			viewProjection_.translation_.x = 0;
		}
	}

	viewProjection_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("followCamera");
	ImGui::DragFloat3("rotate", &viewProjection_.rotation_.x, 0.001f, 0, 10);
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.01f, -100, 100);
	ImGui::DragFloat3("offset", &offset_.x, 0.01f, -100, 100);
	ImGui::Checkbox("startCamera", &isStartCamera_);
	ImGui::End();
#endif
}

#pragma region プライベートな関数

void FollowCamera::GoalAngle() {
	// 2πの定義
	const float pi = 6.28f;

	if (t_ >= 1.0f) {
		t_ = 0;
		easeFrame_ = 0;
		easeCount_++;
	}
	// イージング
	t_ = EaseOutExpo(easeFrame_ / kEndEaseFrame);
	for (int i = 0; i < kMaxEase; i++) {
		if (easeCount_ == i) {
			viewProjection_.rotation_ = Lerp(Vector3(0, pi / kMaxEase * i, 0), Vector3(0, pi / kMaxEase * (i + 1), 0), t_);
		}
		else if (easeCount_ >= kMaxEase) {
			isGoalAngle_ = false;
			t_ = 0;
			easeFrame_ = 0;
			easeCount_ = 0;
		}
	}
}

void FollowCamera::GoalCameraToTargetDist() {
	if (easeCount_ == 0) {
		// イージング
		t_ = EaseOutExpo(easeFrame_ / kEndEaseFrame);
		offset_ = Lerp(Vector3(0, 2.5f, -40), Vector3(0, 2.5f, -30), t_);
	}
	if (t_ >= 1.0f) {
		t_ = 0;
		easeFrame_ = 0;
		isGoalAngle_ = true;
		isGoalMove_ = false;
	}
}

float FollowCamera::EaseOutExpo(float x) {
	if (x == 1) {
		return x = 1;
	}
	else {
		return 1 - static_cast<float>(pow(2, -10 * x));
	}
}

Matrix4x4 FollowCamera::MakeRotateMatrix(const Vector3& radian) {
	Matrix4x4 rotateX{};
	Matrix4x4 rotateY{};
	Matrix4x4 rotateZ{};
	rotateX = MakeRotateXMatrix(radian.x);
	rotateY = MakeRotateYMatrix(radian.y);
	rotateZ = MakeRotateZMatrix(radian.z);

	Matrix4x4 result{};
	result = Multiply(rotateX, Multiply(rotateY, rotateZ));

	return result;
}

#pragma endregion