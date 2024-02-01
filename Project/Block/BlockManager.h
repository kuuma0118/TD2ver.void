#pragma once
#include "Project/Block/Block.h"
#include "Project/Block/HeadBlock.h"
#include "Engine/Components/Input.h"
#include "Engine/Utility/CollisionManager/CollisionManager.h"

// Project
#include "Project/Components/GamePad.h"

enum class Shape {
	shape_I,	//I字ブロック
	shape_T,	//T字ブロック
	shape_S,	//S字ブロック
	shape_O,	//O字ブロック
	shape_J,	//J字ブロック
	shape_L,	//L字ブロック
	shape_ten,	//一つのブロック
	shape_side	//横に連なるブロック
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
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);
	
	///
	/// User Method
	/// 

	/// <summary>
	/// 予測ブロックの１番目
	/// </summary>
	void Shape_one(ViewProjection viewProjection_);
	/// <summary>
	/// 予測ブロックの２番目
	/// </summary>
	void Shape_Second(ViewProjection viewProjection_);

	/// <summary>
	/// 形状の保管
	/// </summary>
	void ShapeManagement();


	/// <summary>
	/// I字ブロック
	/// </summary>
	void Shape_I(Vector3 velocity, int index);

	/// <summary>
	/// T字ブロック
	/// </summary>
	void Shape_T(Vector3 velocity, int index);

	/// <summary>
	/// S字ブロック
	/// </summary>
	void Shape_S(Vector3 velocity, int index);

	/// <summary>
	/// O字ブロック
	/// </summary>
	void Shape_O(Vector3 velocity, int index);

	/// <summary>
	/// J字ブロック
	/// </summary>
	void Shape_J(Vector3 velocity, int index);

	/// <summary>
	/// L字ブロック
	/// </summary>
	void Shape_L(Vector3 velocity, int index);

	/// <summary>
	/// 一つのブロック
	/// </summary>
	void Shape_Ten(Vector3 velocity, int index);

	/// <summary>
	/// 横に連なるブロック
	/// </summary>
	/// <param name="velocity"></param>
	void shape_side(Vector3 velocity, int index);
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
	// GamePad
	GamePad* gamePad_ = nullptr;
	//ビュープロジェクション(カメラ)
	ViewProjection viewProjection_;
	//ワールドトランスフォーム(ブロックの発生場所)
	WorldTransform worldTransform_;
	WorldTransform NextworldTransform_[4];
	// 壁
	WorldTransform wallWorld_[2];
	// 床
	WorldTransform floorWorld_;

	//ブロック
	std::list<Block*> blocks_;
	std::list<HeadBlock*> headblocks_;

	// 当たり判定
	CollisionManager* collisionManager_ = nullptr;

	//ブロックの間隔
	float width = 2.0f;
	float height = 2.2f;

	//フェーズ
	Shape shape_;
	Shape ChangeShape_[3];

	int index_ = rand() % 4;
	int Changeindex_[3] ;

	//3Dモデル
	// ブロック
	std::unique_ptr<Model> model_{};
	// ブロック
	std::unique_ptr<Model> Nextmodel_[4]{};
	// 壁
	std::unique_ptr<Model> wall_[2];
	// 床
	std::unique_ptr<Model> floor_;
	//テクスチャハンドル
	uint32_t BlockTexHandle_ = 0;
	uint32_t hardBlockTexHandle_ = 0;

	// 
	Vector2 clearBlock_[20];

	// ブロックが消えるフラグ
	bool isDelete_;
};

