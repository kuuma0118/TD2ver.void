#include "Player.h"
#include "Engine/Utility/MathStruct.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Engine/Utility/Log.h"

Player::~Player() {

}

void Player::Initialize() {
	// 入力
	input_ = Input::GetInstance();
	gamePad_ = GamePad::GetInstance();
	// モデルの読み込み
	model_.reset(Model::CreateFromOBJ("Resources", "block.obj"));
	assert(model_);

	// ワールド座標の初期化
	worldTransform_.Initialize();
	// 初期位置
	worldTransform_.translation_ = { 0,0,0 };
	// 右向きに設定
	worldTransform_.rotation_ = { 0,(float)M_PI / 2, 0 };
	// worldMatrixに反映
	worldTransform_.UpdateMatrix();

	// 速度ベクトル
	velocity_ = { 0,0,kSpeed };
	// 自機の向きに合わせ、回転
	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);
	// 加速度
	acceleration_ = { 0,-0.05f,0 };
	// ジャンプの初速
	initialVel_ = 0.5f;

	// 進む方向を逆にする信号
	inverseVelSignal_ = false;

	///ImGuiように作った変数
	// ジャンプしたか
	isJump_ = false;
	// 生きてるよ
	isAlive_ = true;

	// 当たり判定の形状を設定
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);

	AABB aabb = {
	{-0.8f,-1.0f,-0.8f},
	{0.8f,1.0f,0.8f}
	};
	SetAABB(aabb);

	hitCounter_ = 0;
}

void Player::Update() {
	// 重力を速度に足す
	velocity_.y += acceleration_.y;

	// 床の判定
	if (worldTransform_.translation_.y <= -5) {
		worldTransform_.translation_.y = -5;
		velocity_.y = 0;
	}

	if (worldTransform_.translation_.x <= -12 || worldTransform_.translation_.x >= 12) {
		inverseVelSignal_ = true;
	}

	/// ふるまい
	// 初期化
	B_Init();
	// 更新処理
	B_Update();

	// 移動に関する処理
	Move();

	// ImGui
	AdjustmentParameter();

	// ワールド座標の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::OnCollision(Collider* collider) {
	float theta = atan2(worldTransform_.translation_.y - collider->GetWorldPosition().y, worldTransform_.translation_.x - collider->GetWorldPosition().x);

	// 上
	if (theta <= -(M_PI / 3.5f) && theta >= -M_PI + (M_PI / 3.5f)) {
		isAlive_ = false;
		worldTransform_.UpdateMatrix();
	}
	// 下
	if (theta >= (M_PI / 5) && theta <= M_PI - (M_PI / 5)) {
		float extrusion = (-GetAABB().min.y + collider->GetAABB().max.y) - (worldTransform_.translation_.y - collider->GetWorldPosition().y);
		worldTransform_.translation_.y += extrusion;
		worldTransform_.UpdateMatrix();
		if (hitCounter_ <= 0) {
			isJump_ = false;
			behaviorRequest_ = Behavior::kLanding;
		}
		velocity_.y = 0;
		hitCounter_++;
	}

	// 左
	if (theta < M_PI / 10.0f && theta > -(M_PI / 10.0f)) {
		float extrusion = (-GetAABB().min.x + collider->GetAABB().max.x) - (worldTransform_.translation_.x - collider->GetWorldPosition().x);
		worldTransform_.translation_.x += extrusion;
		worldTransform_.UpdateMatrix();
		if (!collider->GetIsTopHitAABB()) {
			if (collider->GetIsBottomHitAABB()) {
				isJump_ = true;
			}
		}
		else {
			inverseVelSignal_ = true;
		}
	}
	// 右
	if (theta > M_PI - (M_PI / 10.0f) || theta < -M_PI + (M_PI / 10.0f)) {
		float extrusion = (GetAABB().max.x + (-collider->GetAABB().min.x)) - (collider->GetWorldPosition().x - worldTransform_.translation_.x);
		worldTransform_.translation_.x -= extrusion;
		worldTransform_.UpdateMatrix();
		if (!collider->GetIsTopHitAABB()) {
			if (collider->GetIsBottomHitAABB()) {
				isJump_ = true;
			}
		}
		else {
			inverseVelSignal_ = true;
		}
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 result = {
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]
	};
	return result;
}

/// プライベート関数↓
void Player::Move() {
	// 移動方向を反転する信号が送られた時の処理
	if (inverseVelSignal_) {
		// 方向ベクトル
		Vector3 velocity(0, 0, kSpeed);

		// 後ろを向かせる
		worldTransform_.rotation_.y += (float)M_PI;
		// 2π以上なら0に初期化
		if (worldTransform_.rotation_.y > (float)M_PI * 3.5f) {
			worldTransform_.rotation_.y = (float)M_PI / 2;
		}
		// ワールド座標を更新
		worldTransform_.UpdateMatrix();

		// 向いている方向に進む。ただし,ワールド座標から見てy,z方向は反転させる必要がないので計算から除外
		velocity_.x = TransformNormal(velocity, worldTransform_.matWorld_).x;

		// 信号をoff
		inverseVelSignal_ = false;
	}

	// 移動処理
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

#pragma region ふるまい

void Player::B_Init() {
	if (behaviorRequest_) {
		//  振るまいを変更
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kNormal:
		default:
			B_NormalInit();
			break;
			// ジャンプした瞬間
		case Behavior::kJump:
			B_JumpInit();
			break;
			// 空中
		case Behavior::kAir:
			B_AirInit();
			break;
			// 着地した瞬間
		case Behavior::kLanding:
			B_LandingInit();
			break;
		}
		// 振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}
void Player::B_Update() {
	switch (behavior_) {
	case Behavior::kNormal:
	default:
		B_NormalUpdate();
		break;
		// ジャンプした瞬間
	case Behavior::kJump:
		B_JumpUpdate();
		break;
		// 空中
	case Behavior::kAir:
		B_AirUpdate();
		break;
		// 着地した瞬間
	case Behavior::kLanding:
		B_LandingUpdate();
		break;
	}
}

void Player::B_NormalInit() {
	velocity_.y = 0;
}
void Player::B_NormalUpdate() {
	if (isJump_) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::B_JumpInit() {
	// 速度を初期化
	velocity_.y = 0;
	// 初速を加算
	velocity_.y += initialVel_;
	isJump_ = false;
}
void Player::B_JumpUpdate() {
	behaviorRequest_ = Behavior::kAir;
}

void Player::B_AirInit() {
	hitCounter_ = 0;
}
void Player::B_AirUpdate() {
	if (worldTransform_.translation_.y <= -5) {
		behaviorRequest_ = Behavior::kLanding;
	}
}

void Player::B_LandingInit() {

}
void Player::B_LandingUpdate() {
	behaviorRequest_ = Behavior::kNormal;
}

#pragma endregion

void Player::AdjustmentParameter() {
#ifdef _DEBUG
	ImGui::Begin("Player");

	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Inverse Velocity")) {
		ImGui::DragFloat3("velocity", &velocity_.x, 0.01f, -1.0f, 1.0f);
		if (ImGui::Button("isActive")) {
			inverseVelSignal_ = true;
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Behavior")) {
		if (ImGui::TreeNode("Normal")) {

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Jump")) {
			ImGui::DragFloat("initial velocity", &initialVel_, 0.001f, 0, 50);
			ImGui::DragFloat3("acceleration", &acceleration_.x, 0.001f, -4.0f, 4.0f);
			ImGui::Checkbox("isActive", &isJump_);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	ImGui::Text("IsAlive:%d", isAlive_);

	ImGui::End();
#endif // DEBUG
}
/// プライベート関数↑