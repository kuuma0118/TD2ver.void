#include "Block.h"

Block::Block(){
}

Block::~Block() {

}

void Block::Initialize(WorldTransform worldTransform) {
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.UpdateMatrix();

	texHandle_ = TextureManager::Load("Resources/white.png");

	model_.reset(Model::CreateFromOBJ("Resources/Cube", "scaffolding.obj"));

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	foolflag = false;

	// 当たり判定の形状を設定
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabb = {
		{-0.9999f,-0.9999f,-0.9999f},
		{0.9999f,0.9999f,0.9999f}
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
		ImGui::TreePop();
	}
	ImGui::End();
#endif // DEBUG
}

void Block::OnCollision(Collider* collider) {	 
	if (worldTransform_.translation_.y > collider->GetWorldPosition().y && GetCollisionAttribute() == collider->GetCollisionAttribute()) {
		float extrusion = (-GetAABB().min.y + collider->GetAABB().max.y) - (worldTransform_.translation_.y - collider->GetWorldPosition().y);
		worldTransform_.translation_.y += extrusion;
		worldTransform_.UpdateMatrix();
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