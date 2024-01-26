#include "BlockManager.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager() {
	for (Block* block_ : blocks_) {
		delete block_;
	}
}

void BlockManager::Initialize(CollisionManager* collisionManager) {
	// コリジョンマネージャーを取得
	collisionManager_ = collisionManager;
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	texHandle_ = TextureManager::Load("Resources/uvChecker.png");

#pragma region モデル読み込み

	// ブロック
	model_.reset(Model::CreateFromOBJ("Resources/Cube", "scaffolding.obj"));
	assert(model_);
	// 壁
	for (int i = 0; i < 2; i++) {
		wall_[i].reset(Model::CreateFromOBJ("Resources", "block.obj"));
		assert(wall_[i]);
	}
	// 床
	floor_.reset(Model::CreateFromOBJ("Resources/Cube", "block.obj"));
	assert(floor_);

#pragma endregion

#pragma region ワールド座標の初期化と初期位置を設定

	// ブロックの沸く場所
	worldTransform_.Initialize();
	// 壁
	for (int i = 0; i < 2; i++) {
		wallWorld_[i].Initialize();
		// 大きさ
		wallWorld_[i].scale_ = { 0.1f, 100.0f,2.0f };
	}
	// 左に配置
	wallWorld_[0].translation_.x = -(float)kBlockNumX;
	// 右に配置
	wallWorld_[1].translation_.x = (float)kBlockNumX;
	// 床
	floorWorld_.Initialize();
	floorWorld_.scale_ = { 100.0f,0.1f,2.0f };
	floorWorld_.translation_.y = kMapBottomPos - 1.0f;

#pragma endregion

	// 消える判定をとるブロックのx,y座標
	for (int i = 0; i < 20; i++) {
		clearBlock_[i].x = (2 * i) - (float)(kBlockNumX - 1);
		clearBlock_[i].y = (2 * i) + kMapBottomPos;
	}
	// ブロックが消えるフラグ
	isDelete_ = false;
}

void BlockManager::Update(Vector3 velocity) {
	worldTransform_.UpdateMatrix();
	for (int i = 0; i < 2; i++) {
		wallWorld_[i].UpdateMatrix();
	}
	floorWorld_.UpdateMatrix();

	if (input_->IsPushKeyEnter(DIK_SPACE)) {

		//形状をランダムにする
		//shape_ = Shape(rand() % 6);
		shape_ = Shape::I;

		switch (shape_)
		{
		case Shape::I:
			Shape_I(velocity);
			break;

		case Shape::T:
			Shape_T(velocity);
			break;

		case Shape::S:
			Shape_S(velocity);
			break;

		case Shape::O:
			Shape_O(velocity);
			break;

		case Shape::J:
			Shape_J(velocity);
			break;

		case Shape::L:
			Shape_L(velocity);
			break;
		}
	}

	for (Block* block_ : blocks_) {
		block_->Update();
	}

	// 消せるブロックがあるかをチェックしあれば消す
	CheckAndClearRow();
}

void BlockManager::Draw(ViewProjection viewProjection_) {
	for (Block* block_ : blocks_) {
		block_->Draw(viewProjection_);
	}
	// 壁
	for (int i = 0; i < 2; i++) {
		wall_[i]->Draw(wallWorld_[i], viewProjection_);
	}
	// 床
	floor_->Draw(floorWorld_, viewProjection_);
}

/// <summary>
/// I字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_I(Vector3 velocity) {
#pragma region ブロックの１番
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
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
		newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	#pragma endregion
	
	#pragma region ブロックの３番
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x,velocity.y + width * 2,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	#pragma endregion
	
	#pragma region ブロックの４番
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x,velocity.y + width * 3,velocity.z });
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
void BlockManager::Shape_T(Vector3 velocity) {
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_2->SetworldTransform_({ velocity.x - width,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_2);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_3->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_4->SetworldTransform_({ velocity.x,velocity.y - width,velocity.z });
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
void BlockManager::Shape_S(Vector3 velocity) {
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_1->SetworldTransform_({ velocity.x + width,velocity.y,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_1);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_3->SetworldTransform_({ velocity.x ,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_3);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_4->SetworldTransform_({ velocity.x - width,velocity.y - width ,velocity.z });
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
void BlockManager::Shape_O(Vector3 velocity) {
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
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
void BlockManager::Shape_J(Vector3 velocity) {
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_4->SetworldTransform_({ velocity.x - width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

/// <summary>
///L字ブロック 
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_L(Vector3 velocity) {
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_2->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_3->Initialize(worldTransform_, texHandle_, model_.get());
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
	newBlock_4->Initialize(worldTransform_, texHandle_, model_.get());
	newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

void BlockManager::CheckAndClearRow() {
	for (int i = 0; i < kBlockNumY; i++) {
		// その列にブロックがいくつあるかの確認
		int count = 0;
		for (Block* block : blocks_) {
			if (!block->GetFoolFlag()) {
				if ((int)clearBlock_[i].y == (int)block->GetWorldPosition().y) {
					for (int j = 0; j < kBlockNumX; j++) {
						if ((int)clearBlock_[j].x == (int)block->GetWorldPosition().x) {
							count++;
							block->SetIsAlive(false);
						}
					}
				}
			}
		}

		if (count >= kBlockNumX) {		
			blocks_.remove_if([](Block* block) {
				if (!block->GetIsAlive()) {
					delete block;
					return true;
				}
				return false;
				});
			// コライダーをすべてクリア
			collisionManager_->ClearColliderList();
			// すでに生成されているブロックをコライダーに登録
			for (Block* block : blocks_) {
				collisionManager_->SetColliderList(block);
			}
			// コライダーのすべてが初期化されてしまっているのでplayerを再pushする
			isDelete_ = true;
		}
		else {
			for (Block* block : blocks_) {
				block->SetIsAlive(true);
			}
		}
	}
}