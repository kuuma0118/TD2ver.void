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
	// 空中にいるか
	isAir_ = false;
	// 着地したか
	isLanding_ = false;

	// 当たり判定の形状を設定
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
}

void Player::Update() {
#ifdef _DEBUG
	if (gamePad_->TriggerButton(XINPUT_GAMEPAD_A)) {
		Log("Trigger\n");
		isJump_ = true;
	}
#endif // _DEBUG

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

void Player::OnCollision(const Collider* collider) {
	//inverseVelSignal_ = true;
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
	isLanding_ = false;
}
void Player::B_NormalUpdate() {
	if (isJump_) {
		behaviorRequest_ = Behavior::kJump;
	}
}

void Player::B_JumpInit() {
	// 初速を加算
	velocity_.y += initialVel_;
	isJump_ = false;
}
void Player::B_JumpUpdate() {
	behaviorRequest_ = Behavior::kAir;
}

void Player::B_AirInit() {
	isAir_ = true;
}
void Player::B_AirUpdate() {
	velocity_.y += acceleration_.y;
	if (worldTransform_.translation_.y <= 0) {
		worldTransform_.translation_.y = 0;
		velocity_.y = 0;
		behaviorRequest_ = Behavior::kLanding;
		isAir_ = false;
	}
}

void Player::B_LandingInit() {
	isLanding_ = true;
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
		if (ImGui::TreeNode("Air")) {
			ImGui::Checkbox("isActive", &isAir_);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Landing")) {
			ImGui::Checkbox("isActive", &isLanding_);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();
#endif // DEBUG
}
/// プライベート関数↑