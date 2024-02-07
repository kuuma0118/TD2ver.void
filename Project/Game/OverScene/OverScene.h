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

class GameOverScene : public IScene {
public:
	//トランジションの時間
	static const int kTransitionTime = 60;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameOverScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOverScene() override;

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
	const std::list<Block*>& Getblocks_()const { return blocks_; }

private:// メンバ変数
#pragma region エンジンの基本機能
	//TextureManager
	TextureManager* textureManager_ = nullptr;
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

	//サウンド
	uint32_t soundHandle_ = 0u;
	uint32_t overSoundHandle_ = 0u;

	int soundCount_ = 0;

	//タイトル用のスプライト
	std::unique_ptr<Sprite>  overSprite_ = nullptr;
	//タイトルのテクスチャ
	uint32_t overTextureHandle_ = 0;
	//リスタートUIのスプライト
	std::unique_ptr<Sprite>  RestartUISprite_ = nullptr;
	//リスタートUIのテクスチャ
	uint32_t RestartUITextureHandle_ = 0;
	//リターンタイトルUIのスプライト
	std::unique_ptr<Sprite>  ReturnTitleUISprite_ = nullptr;
	//リターンタイトルUIのテクスチャ
	uint32_t ReturnTitleUITextureHandle_ = 0;
	//トランジション用のスプライト
	std::unique_ptr<Sprite> transitionSprite_ = nullptr;
	//トランジションのテクスチャ
	uint32_t transitionTextureHandle_ = 0;
	//トランジションの色
	Vector4 transitionColor_ = { 0.0f,0.0f,0.0f,1.0f };
	//トランジションのフラグ
	bool isTransition_ = false;
	bool isTransitionEnd_ = false;
	bool isTransition4Title_ = false;
	bool isTransition4Game_ = false;
	//トランジションのタイマー
	float transitionTimer_ = 0;

	//ブロック
	std::list<Block*> blocks_;
	// 自機
	Player* player_;

#pragma endregion
};