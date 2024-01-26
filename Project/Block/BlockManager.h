#pragma once
#include "Project/Block/Block.h"
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
	///
	/// Default Method
	/// 

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
	
	///
	/// User Method
	/// 

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
	/// 横一列に並んだらブロックが消える
	/// </summary>
	void CheckAndClearRow();

	/// Getter

	/// <summary>
	/// ブロックが消えるかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDelete() { return isDelete_; }

	/// Setter

	/// <summary>
	/// ブロックが消えるかを設定
	/// </summary>
	/// <param name="isDelete"></param>
	void SetIsDelete(bool isDelete) { isDelete_ = isDelete; }

private:// 定数
	// マップの左端
	const float kMapLeftPos = 0.0f;
	// マップの最底辺
	const float kMapBottomPos = -5.0f;

	/// 判定をとるブロックの数
	// 横(ブロックが消えるのに必要な数)
	const int kBlockNumX = 13;
	// 縦
	const int kBlockNumY = 20;

private:
	//Input
	Input* input_ = nullptr;
	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;
	// 壁
	WorldTransform wallWorld_[2];
	// 床
	WorldTransform floorWorld_;

	//ブロック
	std::list<Block*> blocks_;

	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//ブロックの間隔
	float width = 2.00f;

	//フェーズ
	Shape shape_ = Shape::I;
	//int num = rand();

	//3Dモデル
	// ブロック
	std::unique_ptr<Model> model_{};
	// 壁
	std::unique_ptr<Model> wall_[2];
	// 床
	std::unique_ptr<Model> floor_;
	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	// 
	Vector2 clearBlock_[20];

	// ブロックが消えるフラグ
	bool isDelete_;
};

