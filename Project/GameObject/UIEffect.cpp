#include "UIEffect.h"

int UIEffect::deadTime = 60;

void UIEffect::Initialize(const Vector3& position, const Vector3& scale) {

	//モデルの作成
	model_.reset(Model::CreateFromOBJ("Resources/UIobj", "untitled.obj"));
	model_->GetDirectionalLight()->SetEnableLighting(false);
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = scale;

	//タイマーの初期化
	DeathTimer_ = deadTime;
}

void UIEffect::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//死亡フラグを立てる
	if (--DeathTimer_ < 0) {
		isDead_ = true;
	}
}

void UIEffect::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 UIEffect::GetWorldPosition() {
	Vector3 position{};
	position.x = worldTransform_.matWorld_.m[3][0];
	position.y = worldTransform_.matWorld_.m[3][1];
	position.z = worldTransform_.matWorld_.m[3][2];
	return position;
}