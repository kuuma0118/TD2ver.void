#pragma once
#include "IScene.h"
/// Engine↓
// Components
#include "Engine/Components/Input.h"
#include "Engine/Components/Audio.h"
#include "Engine/Components/PostProcess.h"
// 3D
#include "Engine/3D/Model/Model.h"
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

	//サウンド
	uint32_t SceneSoundHandle_ = 0u;
	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;

	//ブロックを落とすUIのスプライト
	std::unique_ptr<Sprite>  dropUISprite_ = nullptr;
	//ブロックを落とすUIのテクスチャ
	uint32_t dropUITextureHandle_ = 0;
	//ブロックの移動UIのスプライト
	std::unique_ptr<Sprite>  moovUISprite_ = nullptr;
	//ブロックの移動UIのテクスチャ
	uint32_t moovUITextureHandle_ = 0;
	
	/// チュートリアル用の文字
	// 登れ
	std::unique_ptr<Sprite>  guideGoalLineSprite_ = nullptr;
	uint32_t guideGoalLineTexture_ = 0;
	bool isGuideGoalLine_ = false;
	// 納期から逃げろ
	std::unique_ptr<Sprite>  guideDeadLineSprite_ = nullptr;
	uint32_t guideDeadLineTexture_ = 0;
	bool isGuideDeadLine_ = false;
	// 一列揃うとブロックが消える
	std::unique_ptr<Sprite>  guideDeleteBlockSprite_ = nullptr;
	uint32_t guideDeleteTexture_ = 0;
	bool isGuideDelete_ = false;
	// スタート
	std::unique_ptr<Sprite>  guideStartSprite_ = nullptr;
	uint32_t guideStartTexture_ = 0;
	bool isGuideStart_ = false;
	int guideClearFrame_ = 0;
	// 色
	Vector4 color_;
	// デッドラインの点滅に使用するアルファ値
	float alpha_;
	// デッドラインの点滅速度
	const float kFlashSpeed_ = 1.0f / 10.0f;
	// デッドラインのアルファ値の最低値
	const float kMinAlpha_ = 0.0f;
	// デッドラインのアルファ値の最大値
	const float kMaxAlpha_ = 1.0f;

	// 背景
	std::unique_ptr<Sprite>  backGround_ = nullptr;
	uint32_t backGroundTexture_ = 0;

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

	bool isOpeningCamera_;
	int currentFrame_;

#pragma endregion
};