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
//#include "Project/Block/Block.h"
#include "Project//Block/BlockManager.h"

class GameScene : public IScene {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

private:// プライベートな関数
	// ブロックリストを取得
//	const std::list<Block*>& Getblocks_()const { return blocks_; }

private:// メンバ変数
#pragma region エンジンの基本機能

	//Input
	Input* input_ = nullptr;
	//オーディオクラス
	Audio* audio_ = nullptr;
	//ポストプロセス
	PostProcess* postProcess_ = nullptr;
	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

#pragma endregion

#pragma region ゲームオブジェクト

	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;

	//ブロック
	//std::list<Block*> blocks_;
	BlockManager* blockManager_;

	// 自機
	Player* player_;

#pragma endregion
};