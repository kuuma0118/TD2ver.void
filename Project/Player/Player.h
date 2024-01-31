#pragma once
#include "Engine/Components/Input.h"
#include "Engine//Utility/CollisionManager/Collider.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/3D/Matrix/WorldTransform.h"
#include "Project/Components/GamePad.h"
#include <optional>
#include <memory>

class Player : public Collider
{
public:
	/// 
	/// Default Method
	/// 

	// コンストラクタ
	Player() = default;
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(const ViewProjection& viewProjection);

	// 当たった時の処理
	void OnCollision(Collider* collider) override;

	///
	///	User Method
	///

private:
	// 移動処理
	void Move();

#pragma region ふるまい
	/// Behaviorだと名前が長いのでB_で書いてます
	///
	/// Default Method
	///
	
	// 初期化
	void B_Init();
	// 更新処理
	void B_Update();

	///
	/// User Method
	///
	
	/// 通常状態
	// 初期化
	void B_NormalInit();
	// 更新処理
	void B_NormalUpdate();

	/// ジャンプした瞬間
	// 初期化
	void B_JumpInit();
	// 更新処理
	void B_JumpUpdate();

	/// 空中
	// 初期化
	void B_AirInit();
	// 更新処理
	void B_AirUpdate();

	/// 着地した瞬間
	// 初期化
	void B_LandingInit();
	// 更新処理
	void B_LandingUpdate();
#pragma endregion

	// 調整項目
	void AdjustmentParameter();

public:
	/// Getter
	// 座標
	Vector3 GetPosition() { return worldTransform_.translation_; }
	// ワールドトランスフォーム
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	// ワールド座標
	Vector3 GetWorldPosition()override;
	
	// 生きているかを取得
	bool GetIsAlive() { return isAlive_; }

	///	Setter
	// 生きているかを設定
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }

private:
	// 入力
	Input* input_;
	// GamePad
	GamePad* gamePad_;

	// 移動処理用のワールド座標
	WorldTransform worldTransform_;

	// モデル
	std::unique_ptr<Model> model_;

	// 速度ベクトル
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// ジャンプの初速度
	float initialVel_;

	// 進む方向が反転する信号
	bool inverseVelSignal_;
	// ジャンプしたか
	bool isJump_;
	//// 空中にいるか
	//bool isAir_;
	//// 着地したか
	//bool isLanding_;
	// 生きてるか
	bool isAlive_;

	// 振るまい
	enum class Behavior {
		kNormal,	 // 通常状態
		kJump,	 // ジャンプした瞬間
		kAir,	 // 空中
		kLanding // 着地した瞬間
	};
	Behavior behavior_ = Behavior::kNormal;
	// 次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 連続でヒットしているかの確認
	uint32_t hitCounter_;
private:// 定数
	// 移動量
	const float kSpeed = 0.04f;
};