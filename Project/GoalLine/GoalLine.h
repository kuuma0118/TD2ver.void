#pragma once
#include "3D/Model/Model.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "2D/Sprite.h"

#include "Project/Player/Player.h"
#include <memory>

class GoalLine {
public:
	///
	///	Default Method
	/// 

	// コンストラクタ
	GoalLine() = default;
	// デストラクタ
	~GoalLine() = default;

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
	// ゴールラインに達したかを取得
	bool GetIsGoal() { return isGoal_; }

	// ワールド座標を取得(Vector3)
	Vector3 GetWorldPosition() { return worldTransform_.translation_; }

	/// Setter
	// ゴールラインに達したかを設定
	void SetIsGoal(bool isGoal) { isGoal_ = isGoal; }

	// 自機のアドレスを取得
	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を設定
	void SetWorldPosition(Vector3 pos) { worldTransform_.translation_ = pos; }

private:// プライベートな関数
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

	Player* player_;

	// ワールド座標
	WorldTransform worldTransform_;
	// 自機がゴールラインに達したか
	bool isGoal_;
};