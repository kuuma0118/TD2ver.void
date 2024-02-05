#pragma once
/// Engine↓
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "2D/Sprite.h"

/// Project↓
#include "Project/Player/Player.h"

#include <memory>

class DeadLine {
public:
	///
	///	Default Method
	/// 

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const ViewProjection& viewProjection);

	/// <summary>
	/// 3Dモデルを使用してラインを描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw3DLine(const ViewProjection& viewProjection);

	/// <summary>
	/// スプライトを使用してラインを描画
	/// </summary>
	void Draw2DLine();

	/// 
	/// User Method
	/// 

	/// Getter
	// ワールド座標を取得
	Vector3 GetWorldPosition(){
		Vector3 result = {
			worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2]};
		return result;
	}
	// ワールドトランスフォームを取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// Setter
	// 自機のアドレスを取得
	void SetPlayer(Player* player) { player_ = player; }

	// ブロックが消えているかをアドレスで取得
	void SetIsBlockDelete(bool isDelete) { isBlockDelete_ = isDelete; }

private:// プライベートな関数
	// デッドラインの点滅処理
	void FlashUpdate();

	// ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	// 座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	// ワールド座標からスクリーン座標に変換
	void WorldToScreenConversion(const ViewProjection& viewProjection);

private:
	// スプライト
	std::unique_ptr<Sprite> line2DSprite_;
	// 3Dモデル(デバッグ用)
	std::unique_ptr<Model> line3DModel_;
	// ラインに使用するテクスチャ
	uint32_t lineTexture_;

	// 色
	Vector4 color_;
	// デッドラインの点滅に使用するアルファ値
	float alpha_;

	// 自機
	Player* player_;

	// ワールド座標
	WorldTransform worldTransform_;

	// ブロックが消えたかのフラグ
	bool isBlockDelete_;

private:// 定数
	// デッドラインの上昇速度
	const float kSpeed_ = 0.002f;
	// デッドラインの下がる大きさ
	const float kDownValue = 0.5f;

	// デッドラインの開始位置
	const float kStartDeadLine_ = -7;

	// デッドラインの点滅速度
	const float kFlashSpeed_ = 1.0f / 90.0f;
	// デッドラインのアルファ値の最低値
	const float kMinAlpha_ = 0.4f;
	// デッドラインのアルファ値の最大値
	const float kMaxAlpha_ = 1.0f;
};