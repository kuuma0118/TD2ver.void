#pragma once
#include "Engine/Components/Input.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/2D/ImGuiManager.h"
#include "Engine/Base/TextureManager.h"
#include "Engine/Utility/CollisionManager/Collider.h"

class Block :public Collider
{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Block();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Block();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform worldTransform, uint32_t texHandle,  Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

	///
	/// User Method
	/// 

	// 純粋仮想関数
	void OnCollision(Collider* collider) override;

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

	// ImGui
	void AdjustmentParameter();
	//void OnCollision();
	Vector3 GetworldTransform_() { return worldTransform_.translation_; }

	void SetworldTransform_(Vector3 worldTransform) {this->worldTransform_.translation_ = worldTransform;}

private: // メンバ変数
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dモデル
	Model* model_{};

	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;

	//落ちるフラグ
	bool foolflag = false;

	//速度
	float foolSpeed_ = 0.2f;
};

