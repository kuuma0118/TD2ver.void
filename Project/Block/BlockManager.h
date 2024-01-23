#pragma once
#include "Project/Block/Block.h"
#include "Project/Block/HeadBlock.h"
#include "Engine/Components/Input.h"


// Utility
#include "Engine/Utility/CollisionManager/CollisionManager.h"


enum class Shape {
	I,	//I字ブロック
	T,	//T字ブロック
	S,	//S字ブロック
	O,	//O字ブロック
	J,	//J字ブロック
	L,	//L字ブロック
};


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

	/// <summary>
	/// S字ブロック
	/// </summary>
	void Shape_S(Vector3 velocity);

	/// <summary>
	/// O字ブロック
	/// </summary>
	void Shape_O(Vector3 velocity);

	/// <summary>
	/// J字ブロック
	/// </summary>
	void Shape_J(Vector3 velocity);

	/// <summary>
	/// L字ブロック
	/// </summary>
	void Shape_L(Vector3 velocity);

	/// <summary>
	/// ブロックの選別
	/// </summary>
	void Shape_rand();

private:
	//Input
	Input* input_ = nullptr;
	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;

	//ブロック
	std::list<Block*> blocks_;
	std::list<HeadBlock*> headblocks_;


	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//ブロックの間隔
	float width = 2.00f;

	//フェーズ
	Shape shape_ = Shape::I;
	//int num = rand();

	//3Dモデル
	std::unique_ptr<Model> model_{};

	//テクスチャハンドル
	uint32_t BlockTexHandle_ = 0;
	uint32_t hardBlockTexHandle_ = 0;
};

