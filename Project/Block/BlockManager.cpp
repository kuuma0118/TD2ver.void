#include "BlockManager.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager(){
	for (Block* block_ : blocks_) {
		delete block_;
	}
}

void BlockManager::Initialize(CollisionManager* collisionManager){
	worldTransform_.Initialize();
	collisionManager_ = collisionManager;
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	
}

void BlockManager::Update(Vector3 velocity){
	worldTransform_.UpdateMatrix();
	if (input_->IsPushKeyEnter(DIK_SPACE)) {
		///ここをランダムにする
		Shape_L(velocity);
	}
	for (Block* block_ : blocks_) {
		block_->Update();
	}
}

void BlockManager::Draw(ViewProjection viewProjection_){
	for (Block* block_ : blocks_) {
		block_->Draw(viewProjection_);
	}
}

/// <summary>
/// I字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_I(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x,velocity.y - width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x,velocity.y - width- width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

/// <summary>
/// T字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_T(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x-width,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x+ width,velocity.y ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x,velocity.y- width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

/// <summary>
/// S字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_S(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x+width,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x ,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x ,velocity.y- width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x- width,velocity.y- width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

/// <summary>
/// O字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_O(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x ,velocity.y - width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

/// <summary>
/// J字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_J(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x ,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x ,velocity.y - width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x - width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

void BlockManager::Shape_L(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_);
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_);
	newBlock_2->SetworldTransform_({ velocity.x ,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_);
	newBlock_3->SetworldTransform_({ velocity.x ,velocity.y - width,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_);
	newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

