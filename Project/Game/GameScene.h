#pragma once
#include "IScene.h"
/// Engine↓
// Components
#include "Engine/Components/Input.h"
#include "Engine/Components/Audio.h"
#include "Engine/Components/PostProcess.h"
// 3D
#include "Engine/3D/Model/Model.h"
#include "3D/Model/ParticleModel.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
// 2D
#include "Engine/2D/Sprite.h"
// Utility
#include "Engine/Utility/CollisionManager/CollisionManager.h"

/// Project↓
// GameObject
#include "Project/Player/Player.h"
#include "Project/Block/Block.h"
#include "Project/Block/BlockManager.h"
#include "Project/GoalLine/GoalLine.h"
#include "Project/DeadLine/DeadLine.h"
#include "Project/FollowCamera/FollowCamera.h"

#include <memory>

class GameScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager) override;

private:// メンバ変数
#pragma region エンジンの基本機能

	//Input
	Input* input_ = nullptr;
	//オーディオクラス
	Audio* audio_ = nullptr;
	//ポストプロセス
	PostProcess* postProcess_ = nullptr;
	// 当たり判定
	std::unique_ptr<CollisionManager> collisionManager_;

#pragma endregion

#pragma region ゲームオブジェクト

	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;

	//スプライト
	std::unique_ptr<Sprite> sprite1_ = nullptr;
	std::unique_ptr<Sprite> sprite2_ = nullptr;
	Vector4 spriteColor1_{ 1.0f,1.0f,1.0f,1.0f };
	Vector4 spriteColor2_{ 1.0f,1.0f,1.0f,1.0f };

	//モデル
	std::unique_ptr<Model> model1_ = nullptr;
	std::unique_ptr<Model> model2_ = nullptr;
	Vector4 modelColor1_{ 1.0f,1.0f,1.0f,1.0f };
	Vector4 modelColor2_{ 1.0f,1.0f,1.0f,1.0f };

	//ワールドトランスフォーム
	WorldTransform worldTransform1_{};
	WorldTransform worldTransform2_{};

	//ブロック
	std::unique_ptr<Block> block_ = nullptr;

	// 自機に追従するカメラ
	std::unique_ptr<FollowCamera> followCamera_;
	// 自機
	std::unique_ptr<Player> player_;
	//ブロック
	std::unique_ptr<BlockManager> blockManager_;
	// ゴールライン
	std::unique_ptr<GoalLine> goalLine_;
	// デッドライン
	std::unique_ptr<DeadLine> deadLine_;

#pragma endregion
};