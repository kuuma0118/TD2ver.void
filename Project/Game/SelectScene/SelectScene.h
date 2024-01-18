#pragma once
#include "IScene.h"
#include "Base/TextureManager.h"
#include "Components/Audio.h"
#include "Components/Input.h"
#include "2D/ImGuiManager.h"
#include "3D/DebugCamera/DebugCamera.h"

#include "3D/Model/Model.h"
#include "3D/Model/ParticleModel.h"
#include "2D/Sprite.h"
#include "3D/Matrix/WorldTransform.h"
#include "3D/Matrix/ViewProjection.h"
#include "Components/Particle/ParticleSystem.h"

#include <memory>
#include <Utility/CollisionManager/CollisionManager.h>

class GameSelectScene : public IScene
{
public:
	//トランジションの時間
	static const int kTransitionTime = 60;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameSelectScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameSelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager)override;

private:
	//TextureManager
	TextureManager* textureManager_ = nullptr;
	//Audio
	Audio* audio_ = nullptr;
	//Input
	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	//パーティクルモデルの作成
	uint32_t textureHandle_ = 0;
	std::unique_ptr<ParticleModel> particleModel_ = nullptr;
	//パーティクル
	std::unique_ptr<ParticleSystem> particleSystem_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_{};

	//サウンド
	uint32_t soundHandle_ = 0u;
	uint32_t titleSoundHandle_ = 0u;

	int soundCount_ = 0;

	//ワールドトランスフォーム
	WorldTransform playerWorldTransform_{};
	WorldTransform weaponWorldTransform_{};

	//トランジション用のスプライト
	std::unique_ptr<Sprite> transitionSprite_ = nullptr;
	//トランジションのテクスチャ
	uint32_t transitionTextureHandle_ = 0;
	//トランジションの色
	Vector4 transitionColor_ = { 0.0f,0.0f,0.0f,1.0f };
	//トランジションのフラグ
	bool isTransition_ = false;
	bool isTransitionEnd_ = false;
	//トランジションのタイマー
	float transitionTimer_ = 0;

	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;
};

