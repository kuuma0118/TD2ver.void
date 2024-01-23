#include "HeadBlock.h"
#include <cassert>

HeadBlock::HeadBlock(){
}

HeadBlock::~HeadBlock(){
}

void HeadBlock::Initialize(WorldTransform worldTransform, uint32_t texHandle, Model* model){
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.UpdateMatrix();

	texHandle_ = texHandle;
	model_ = model;
	assert(model_);

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	foolflag = false;

	// 当たり判定の形状を設定
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = {
		{-0.99f,-0.99f,-0.99f},
		{0.99f,0.99f,0.99f}
	};
	SetAABB(aabb);
}

void HeadBlock::Update(){
	viewProjection_.UpdateMatrix();

	AdjustmentParameter();

	worldTransform_.translation_.y -= foolSpeed_;

	if (worldTransform_.translation_.y <= -10) {
		float Y = worldTransform_.translation_.y - (-10);
		worldTransform_.translation_.y -= Y;
	}
	worldTransform_.UpdateMatrix();
}

void HeadBlock::Draw(ViewProjection viewProjection_){
	model_->Draw(worldTransform_, viewProjection_, texHandle_);
}

void HeadBlock::AdjustmentParameter(){
#ifdef _DEBUG
	ImGui::Begin("Block");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &worldTransform_.scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // DEBUG
}

void HeadBlock::OnCollision(const Collider* collider){
	foolSpeed_ = 0.0f;
}

Vector3 HeadBlock::GetWorldPosition(){
	Vector3 result = {
		worldTransform_.matWorld_.m[3][0],
		worldTransform_.matWorld_.m[3][1],
		worldTransform_.matWorld_.m[3][2]
	};
	return result;
}