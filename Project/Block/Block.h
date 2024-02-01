#pragma once
#include "Engine/Components/Input.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/2D/ImGuiManager.h"
#include "Engine/Base/TextureManager.h"
#include "Engine/Utility/CollisionManager/Collider.h"
#include "3D/Model/ParticleModel.h"
#include "Components/Particle/ParticleSystem.h"

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

	void DrawParticle(const ViewProjection& viewProjection);

	///
	/// User Method
	/// 

	/// 純粋仮想関数
	// 衝突応答
	void OnCollision(Collider* collider) override;
	// ワールド座標を取得
	Vector3 GetWorldPosition() override;

	// ImGui
	void AdjustmentParameter();
 
	/// Getter
	// ワールド座標を取得
	Vector3 GetworldTransform_() { return worldTransform_.translation_; }
	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
	// 落ちている最中かを取得
	bool GetFoolFlag() { return foolflag; }
	// 消えないブロックを取得
	inline bool GetIsHardBlock() { return isHardBlock_; }

	/// Setter
	// ワールド座標を設定
	void SetworldTransform_(Vector3 worldTransform) {this->worldTransform_.translation_ = worldTransform;}
	// 生存フラグを設定
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	// 消えないブロックを設定
	inline void SetIsHardBlock(bool isActive) { isHardBlock_ = isActive; }

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
	// 生存フラグ
	bool isAlive_;

	//速度
	float foolSpeed_ = 0.2f;

	// 消えないブロック
	bool isHardBlock_;

	//パーティクル
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
};

