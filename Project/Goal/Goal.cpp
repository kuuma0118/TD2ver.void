#include "Goal.h"

void Goal::Initialize() {
	// 使用するテクスチャを読み込む
	lineTexture_ = TextureManager::Load("Resources/white.png");
	// 実体を生成
	line2DSprite_.reset(Sprite::Create(lineTexture_, Vector2{0,0}));
	// サイズ設定
	line2DSprite_->SetSize(Vector2(1280, 4));
	// RGBA設定
	line2DSprite_->SetColor(Vector4(0, 0, 1, 0.6f));

	// 3Dモデル
	line3DModel_.reset(Model::CreateFromOBJ("Resources", "block.obj"));
	// 3Dモデルの初期設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,10,0 };
	worldTransform_.scale_ = { 100,0.1f,1 };
	worldTransform_.UpdateMatrix();

	// ゴールフラグ
	isGoal_ = false;
}

void Goal::Update(const ViewProjection& viewProjection) {
	// ゴールラインより自機が上に行ったらクリア
	if (player_->GetWorldPosition().y <= worldTransform_.translation_.y) {
		isGoal_ = true;
	}

	worldTransform_.UpdateMatrix();

	// 3Dデッドラインからスクリーン座標を算出
	WorldToScreenConversion(viewProjection);
}

void Goal::Draw3DLine(const ViewProjection& viewProjection) {
#ifdef _DEBUG
	// ワールド座標上の3Dライン
	line3DModel_->Draw(worldTransform_, viewProjection, lineTexture_);
#endif // _DEBUG
}

void Goal::Draw2DLine() {
	line2DSprite_->Draw();
}

#pragma region プライベートな関数

Matrix4x4 Goal::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
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

Vector3 Goal::Transform(const Vector3& vector, const Matrix4x4& matrix) {
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

void Goal::WorldToScreenConversion(const ViewProjection& viewProjection) {
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