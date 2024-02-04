#include "DeadLine.h"

void DeadLine::Initialize() {
	// 使用するテクスチャを読み込む
	lineTexture_ = TextureManager::Load("Resources/white.png");
	// 実体を生成
	line2DSprite_.reset(Sprite::Create(lineTexture_, Vector2{ 0,0 }));
	// サイズ設定
	line2DSprite_->SetSize(Vector2(1280, 4));
	// 色設定
	color_ = { 1,0,0,kMinAlpha_ };
	// デッドラインの点滅に使用するアルファ値設定
	alpha_ = 0.0f;
	// RGBA設定
	line2DSprite_->SetColor(color_);

	// 3Dモデル
	line3DModel_.reset(Model::CreateFromOBJ("Resources", "block.obj"));
	// 3Dモデルの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,kStartDeadLine_,0 };
	worldTransform_.scale_ = { 100,0.1f,1 };
	worldTransform_.UpdateMatrix();

	isBlockDelete_ = false;
}

void DeadLine::Update(const ViewProjection& viewProjection) {
	// 敗北条件
	if (player_->GetWorldPosition().y + player_->GetAABB().max.y + 0.25f <= worldTransform_.translation_.y && player_->GetWorldPosition().y >= -5) {
		player_->SetIsAlive(false);
	}

	// 上昇処理
	worldTransform_.translation_.y += kSpeed_;
	worldTransform_.UpdateMatrix();

	// ブロックが消されたらデッドラインを下げる
	if (isBlockDelete_) {
		worldTransform_.translation_.y -= kDownValue;
	}

	// 3Dデッドラインからスクリーン座標を算出
	WorldToScreenConversion(viewProjection);

	// 点滅処理
	FlashUpdate();
}

void DeadLine::Draw3DLine(const ViewProjection& viewProjection) {
#ifdef _DEBUG
	// ワールド座標上の3Dライン
	line3DModel_->Draw(worldTransform_, viewProjection, lineTexture_);
#endif // _DEBUG
}

void DeadLine::Draw2DLine() {
	line2DSprite_->Draw();
}

#pragma region プライベートな関数

void DeadLine::FlashUpdate() {
	// alphaが上限値までいったら0にする
	if (alpha_ >= kMaxAlpha_) {
		alpha_ = 0.0f;
	}
	// デッドラインを点滅
	alpha_ += kFlashSpeed_;
	color_.w = kMinAlpha_ + sinf(alpha_) * (kMaxAlpha_ - kMinAlpha_);
	// アルファ値を反映
	line2DSprite_->SetColor(color_);
}

Matrix4x4 DeadLine::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result;

	result.m[0][0] = width / 2.0f;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = -(height / 2.0f);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0;

	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1;

	return result;
}

Vector3 DeadLine::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

void DeadLine::WorldToScreenConversion(const ViewProjection& viewProjection) {
	// 3Dデッドラインのワールド座標を取得
	Vector3 pos = GetWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport_ =
		MakeViewportMatrix(0, 0, (float)WinApp::kClientWidth, (float)WinApp::kClientHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport{};
	matViewProjectionViewport =
		Multiply(viewProjection.matView_, Multiply(viewProjection.matProjection_, matViewport_));
	// ワールド→スクリーン座標変換
	pos = Transform(pos, matViewProjectionViewport);
	// スプライトのデッドラインに座標設定
	line2DSprite_->SetPosition(Vector2(0, pos.y));
}

#pragma endregion