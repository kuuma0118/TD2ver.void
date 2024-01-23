#include "BlockManager.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager(){
	for (Block* block_ : blocks_) {
		delete block_;
	}

	for (HeadBlock* headblock_ : headblocks_) {
		delete headblock_;
	}
}

void BlockManager::Initialize(CollisionManager* collisionManager){
	worldTransform_.Initialize();
	collisionManager_ = collisionManager;
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	BlockTexHandle_ = TextureManager::Load("Resources/uvChecker.png");
	hardBlockTexHandle_= TextureManager::Load("Resources/cube.jpg");

	model_.reset(Model::CreateFromOBJ("Resources/Cube", "scaffolding.obj"));
	assert(model_);
}

void BlockManager::Update(Vector3 velocity){
	worldTransform_.UpdateMatrix();

	if (input_->IsPushKeyEnter(DIK_SPACE)) {

		//形状をランダムにする
		//shape_ = Shape( rand() % 6);
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
	for (HeadBlock* headblock_ : headblocks_) {
		headblock_->Update();
	}
}

void BlockManager::Draw(ViewProjection viewProjection_){
	for (Block* block_ : blocks_) {
		block_->Draw(viewProjection_);
	} 
	for (HeadBlock* headblock_ : headblocks_) {
		headblock_->Draw(viewProjection_);
	}
}

/// <summary>
/// I字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_I(Vector3 velocity){
#pragma region ブロックの１番
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
	blocks_.push_back(newBlock_1);
	collisionManager_->SetColliderList(newBlock_1);

	/*HeadBlock* newheadBlock_1 = new HeadBlock();
	newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
	newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });*/
	//リストに登録
	//headblocks_.push_back(newheadBlock_1);
	// 当たり判定に追加
	//collisionManager_->SetColliderList(newheadBlock_1);
#pragma endregion

#pragma region ブロックの２番
	// 実体生成
	Block* newBlock_2 = new Block();
	// 初期化
	newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newBlock_2->SetworldTransform_({ velocity.x,velocity.y+ width,velocity.z });
	blocks_.push_back(newBlock_2);
	collisionManager_->SetColliderList(newBlock_2);

	/*HeadBlock* newheadBlock_2 = new HeadBlock();
	newheadBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newheadBlock_2->SetworldTransform_({ velocity.x,velocity.y,velocity.z });*/
	//リストに登録
	//headblocks_.push_back(newheadBlock_2);
	// 当たり判定に追加
	//collisionManager_->SetColliderList(newheadBlock_2);
#pragma endregion

#pragma region ブロックの３番
	// 実体生成
	Block* newBlock_3 = new Block();
	// 初期化
	newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newBlock_3->SetworldTransform_({ velocity.x,velocity.y + width*2,velocity.z });
	blocks_.push_back(newBlock_3);
	collisionManager_->SetColliderList(newBlock_3);

	/*eadBlock* newheadBlock_3 = new HeadBlock();
	newheadBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newheadBlock_3->SetworldTransform_({ velocity.x,velocity.y - width,velocity.z });*/
	//リストに登録
	//headblocks_.push_back(newheadBlock_3);
	// 当たり判定に追加
	//collisionManager_->SetColliderList(newheadBlock_3);
#pragma endregion

#pragma region ブロックの４番
	// 実体生成
	Block* newBlock_4 = new Block();
	// 初期化
	newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newBlock_4->SetworldTransform_({ velocity.x,velocity.y + width*3,velocity.z });
	blocks_.push_back(newBlock_4);
	collisionManager_->SetColliderList(newBlock_4);


	/*HeadBlock* newheadBlock_4 = new HeadBlock();
	newheadBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
	newheadBlock_4->SetworldTransform_({ velocity.x,velocity.y - width - width,velocity.z });*/
	//リストに登録
	//headblocks_.push_back(newheadBlock_4);
	// 当たり判定に追加
	//collisionManager_->SetColliderList(newheadBlock_4);
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
	newBlock_1->Initialize(worldTransform_, BlockTexHandle_,model_.get());
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
	newBlock_2->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_3->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_4->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_1->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_2->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_3->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_4->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_1->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_2->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_3->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_4->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_1->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_2->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_3->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_4->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
void BlockManager::Shape_L(Vector3 velocity){
#pragma region ブロックの１番
	// 実体生成
	Block* newBlock_1 = new Block();
	// 初期化
	newBlock_1->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_2->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_3->Initialize(worldTransform_,BlockTexHandle_,model_.get());
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
	newBlock_4->Initialize(worldTransform_,BlockTexHandle_,model_.get());
	newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y - width ,velocity.z });
	//リストに登録
	blocks_.push_back(newBlock_4);
	// 当たり判定に追加
	collisionManager_->SetColliderList(newBlock_4);
#pragma endregion
}

void BlockManager::Shape_rand(){
	
}

