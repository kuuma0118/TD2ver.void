#include "Block.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

Block::Block(){
}

Block::~Block() {
	//delete model_;
}

void Block::Initialize(WorldTransform worldTransform, uint32_t texHandle, Model* model) {
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.UpdateMatrix();

	texHandle_ = texHandle;
	model_ = model;
	assert(model_);

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	foolflag = true;

	// 生存フラグ
	isAlive_ = true;

	// 当たり判定の形状を設定
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = {
		{-1.0f,-1.0f,-1.0f},
		{1.0f,1.0f,1.0f}
	};
	SetAABB(aabb);
}

void Block::Update() {
	viewProjection_.UpdateMatrix();

	AdjustmentParameter();

	worldTransform_.translation_.y -= foolSpeed_;

	if (worldTransform_.translation_.y <= -5) {
		float floor = worldTransform_.translation_.y - (-5);
		worldTransform_.translation_.y -= floor;
		foolflag = false;
	}
	worldTransform_.UpdateMatrix();
}

void Block::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, texHandle_);
}

void Block::AdjustmentParameter()
{
#ifdef _DEBUG
	ImGui::Begin("Block");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f, 0, 10);
		ImGui::Text("isTopHit:%d", GetIsTopHitAABB());
		ImGui::TreePop();
	}
	ImGui::End();
#endif // DEBUG
}

void Block::OnCollision(Collider* collider) {	 
	float theta = atan2(worldTransform_.translation_.y - collider->GetWorldPosition().y, worldTransform_.translation_.x - collider->GetWorldPosition().x);

	// 下
	if (theta >= (M_PI / 4) && theta <= M_PI - (M_PI / 4)) {
		float extrusion = (-GetAABB().min.y + collider->GetAABB().max.y) - (worldTransform_.translation_.y - collider->GetWorldPosition().y);
		worldTransform_.translation_.y += extrusion;
		worldTransform_.UpdateMatrix();
		foolflag = false;
	}
	else {
		foolflag = true;
	}

	// 上
	if (theta <= -(M_PI / 4) && theta >= -M_PI + (M_PI / 4)) {	
		worldTransform_.UpdateMatrix();
		if (GetCollisionAttribute() == collider->GetCollisionAttribute()) {
			SetIsTopHitAABB(true);
		}
	}

	// 右
	if (theta < M_PI / 5 && theta > -(M_PI / 5)) {
		float extrusion = (-GetAABB().min.x + collider->GetAABB().max.x) - (worldTransform_.translation_.x - collider->GetWorldPosition().x);
		worldTransform_.translation_.x += extrusion;
		worldTransform_.UpdateMatrix();
		if (GetCollisionAttribute() == collider->GetCollisionAttribute()) {
			SetIsRightHitAABB(true);
		}
	}
	// 左
	if (theta > M_PI - (M_PI / 5) || theta < -M_PI + (M_PI / 5)) {
		float extrusion = (GetAABB().max.x + (-collider->GetAABB().min.x)) - (collider->GetWorldPosition().x - worldTransform_.translation_.x);
		worldTransform_.translation_.x -= extrusion;
		worldTransform_.UpdateMatrix();
		if (GetCollisionAttribute() == collider->GetCollisionAttribute()) {
			SetIsLeftHitAABB(true);
		}
	}
}

Vector3 Block::GetWorldPosition() {
	Vector3 result = {
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]
	};
	return result;
}