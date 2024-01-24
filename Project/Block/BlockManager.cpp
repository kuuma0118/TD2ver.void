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

	worldTransform_.translation_.y = 8.0f;

	for (int i = 0; i < 3; i++) {
		ChangeShape_[i] = Shape(rand() % 8);
	}
	shape_ = ChangeShape_[0];

	assert(model_);
}

void BlockManager::Update(){
	worldTransform_.UpdateMatrix();

	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		worldTransform_.translation_.x += 2.00f;
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT)) {
		worldTransform_.translation_.x -= 2.00f;
	}


	if (input_->IsPushKeyEnter(DIK_SPACE)) {
		shape_ = ChangeShape_[0];
		ChangeShape_[0] = ChangeShape_[1];
		ChangeShape_[1] = ChangeShape_[2];
		ChangeShape_[2] = Shape(rand() % 8);


		//shape_ = ChangeShape_[0];

		//形状をランダムにする
	//	shape_ = Shape( rand() % 8);
	//	shape_ = Shape::shape_side;
		

		switch (shape_)
		{
		case Shape::shape_I:
			Shape_I(worldTransform_.translation_);
			break;

		case Shape::shape_T:
			Shape_T(worldTransform_.translation_);
			break;

		case Shape::shape_S:
			Shape_S(worldTransform_.translation_);
			break;

		case Shape::shape_O:
			Shape_O(worldTransform_.translation_);
			break;

		case Shape::shape_J:
			Shape_J(worldTransform_.translation_);
			break;

		case Shape::shape_L:
			Shape_L(worldTransform_.translation_);
			break;
		case Shape::shape_ten:
			Shape_Ten(worldTransform_.translation_);
			break;
		case Shape::shape_side:
			shape_side(worldTransform_.translation_);
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
	model_->Draw(worldTransform_, viewProjection_, BlockTexHandle_);

#ifdef _DEBUG
	
#endif // DEBUG
}

/// <summary>
/// I字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_I(Vector3 velocity){

	int index = rand()%4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x,velocity.y + width * 2,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x,velocity.y + width * 2,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x,velocity.y + width * 3,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x,velocity.y + width * 3 ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
#pragma endregion
}

/// <summary>
/// T字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_T(Vector3 velocity){

	int index = rand() % 4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x+ width,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x+ width,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x- width,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x- width,velocity.y + width  ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
#pragma endregion
}

/// <summary>
/// S字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_S(Vector3 velocity){

	int index = rand() % 4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x- width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x- width,velocity.y ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + width  ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
#pragma endregion
}

/// <summary>
/// O字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_O(Vector3 velocity){

	int index = rand() % 4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + width  ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
}

/// <summary>
/// J字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_J(Vector3 velocity){

	int index = rand() % 4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x - width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x - width,velocity.y ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + width*2,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x ,velocity.y + width *2 ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
}

/// <summary>
///L字ブロック 
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_L(Vector3 velocity){

	int index = rand() % 4;

#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		HeadBlock* newheadBlock_3 = new HeadBlock();
		newheadBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_3->SetworldTransform_({ velocity.x ,velocity.y + width,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + width*2,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		HeadBlock* newheadBlock_4 = new HeadBlock();
		newheadBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_4->SetworldTransform_({ velocity.x ,velocity.y + width *2 ,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_4);
	}
}

/// <summary>
/// 
/// </summary>
/// /// <param name="velocity"></param>
void BlockManager::Shape_Ten(Vector3 velocity){
	int index = rand() % 6;

#pragma region ブロックの１番
	if (index == 0 || index == 1 || index == 2 || index == 3) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion
}

/// <summary>
/// 横に連なるブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::shape_side(Vector3 velocity){
	int index = rand() % 6;

#pragma region ブロックの１番
	if (index == 0 || index == 1 || index == 2 || index == 3) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		HeadBlock* newheadBlock_1 = new HeadBlock();
		newheadBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index == 0 || index == 1 || index == 2 || index == 3) {
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x+ width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		HeadBlock* newheadBlock_2 = new HeadBlock();
		newheadBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, model_.get());
		newheadBlock_2->SetworldTransform_({ velocity.x+ width,velocity.y,velocity.z });
		//リストに登録
		headblocks_.push_back(newheadBlock_2);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newheadBlock_2);
	}
#pragma endregion
}

