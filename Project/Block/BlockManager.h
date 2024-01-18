#pragma once
#include "Project/Block/Block.h"
#include "Engine/Components/Input.h"


// Utility
#include "Engine/Utility/CollisionManager/CollisionManager.h"

class BlockManager{
	

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	BlockManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BlockManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(CollisionManager* collisionManager_);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(Vector3 velocity);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);
	
	/// <summary>
	/// I字ブロック
	/// </summary>
	void Shape_I(Vector3 velocity);

	/// <summary>
	/// T字ブロック
	/// </summary>
	void Shape_T(Vector3 velocity);
private:

	//Input
	Input* input_ = nullptr;
	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;

	//ブロック
	std::list<Block*> blocks_;

	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//ブロックの間隔
	float width = 2.001f;
	//int num = rand();
};

