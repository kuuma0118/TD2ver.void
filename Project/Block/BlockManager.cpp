#include "BlockManager.h"

BlockManager::BlockManager()
{
}

BlockManager::~BlockManager() {
	for (Block* block_ : blocks_) {
		delete block_;
	}

	for (HeadBlock* headblock_ : headblocks_) {
		delete headblock_;
	}
}

void BlockManager::Initialize(CollisionManager* collisionManager) {
	collisionManager_ = collisionManager;
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	// ゲームパッドのインスタンスを生成
	gamePad_ = GamePad::GetInstance();
	//オーディオクラスのインスタンスを取得
	audio_ = Audio::GetInstance();

	soundHandle_[0] = audio_->SoundLoadWave("Resources/Sounds/BlockDead.wav");
	soundHandle_[1] = audio_->SoundLoadWave("Resources/Sounds/BlockTrans.wav");

	BlockTexHandle_ = TextureManager::Load("Resources/white.png");
	hardBlockTexHandle_ = TextureManager::Load("Resources/gray.png");
	fallingRangeTexHandle_ = TextureManager::Load("Resources/white.png");

#pragma region モデル読み込み
	// ブロック
	model_.reset(Model::CreateFromOBJ("Resources/Cube", "scaffolding.obj"));
	//model_->GetDirectionalLight()->SetEnableLighting(false);
	assert(model_);
	// 消えないブロック
	hardBlockModel_.reset(Model::CreateFromOBJ("Resources/HardBlock", "HardBlock.obj"));
	//hardBlockModel_->GetDirectionalLight()->SetEnableLighting(false);
	assert(hardBlockModel_);
	for (int i = 0; i < 4; i++) {
		Nextmodel_[i].reset(Model::CreateFromOBJ("Resources/Cube", "scaffolding.obj"));
	}
	// 壁
	for (int i = 0; i < 2; i++) {
		wall_[i].reset(Model::CreateFromOBJ("Resources", "block.obj"));
		assert(wall_[i]);
	}
	// 床
	floor_.reset(Model::CreateFromOBJ("Resources/Cube", "block.obj"));
	assert(floor_);
	// 落下予測範囲
	fallingRange_.reset(Model::CreateFromOBJ("Resources/Cube", "block.obj"));
	assert(fallingRange_);

#pragma endregion

#pragma region ワールド座標の初期化と初期位置を設定
	// ブロックの沸く場所
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 15.0f;
	for (int i = 0; i < 4; i++) {
		NextworldTransform_[i].Initialize();
	}

	srand((unsigned int)time(nullptr));
	for (int i = 0; i < 3; i++) {
		ChangeShape_[i] = Shape(rand() % 8);
	}
	for (int i = 0; i < 3; i++) {
		Changeindex_[i] = rand() % 10;
	}
	shape_ = ChangeShape_[0];
	index_ = Changeindex_[0];

	// 壁
	for (int i = 0; i < 2; i++) {
		wallWorld_[i].Initialize();
		// 座標
		wallWorld_[i].translation_.y = 8;
		// 大きさ
		wallWorld_[i].scale_ = { 0.1f, 14.0f,2.0f };
	}
	// 左に配置
	wallWorld_[0].translation_.x = -(float)kBlockNumX;
	// 右に配置
	wallWorld_[1].translation_.x = (float)kBlockNumX;
	// 床
	floorWorld_.Initialize();
	floorWorld_.scale_ = { 13.0f,0.1f,2.0f };
	floorWorld_.translation_.y = kMapBottomPos - 1.0f;
	// 落下予測地点
	for (int i = 0; i < 4; i++) {
		fallingPoint_[i].Initialize();
		fallingPoint_[i].scale_ = { 1.0f, 50.0f, 0.001f };
	}

#pragma endregion

	// 消える判定をとるブロックのx,y座標
	for (int i = 0; i < 20; i++) {
		clearBlock_[i].x = (2 * i) - (float)(kBlockNumX - 1);
		clearBlock_[i].y = (2 * i) + kMapBottomPos;
	}

	// ブロックが消えるフラグ
	isDelete_ = false;
	// 落下予測地点を表示するか
	isFallingPoint_ = true;
	// 次に落下するブロックがマップ内にあるか
	isNextBlockInMap_ = true;
	// 今ブロックを落下できるか
	isDropBlock_ = true;
	// ブロックの落下クールタイム
	dropCoolTime_ = kDropCoolTime;

	// 落下予測範囲を半透明にする
	fallingRange_->GetMaterial()->SetColor(Vector4{ 1,1,1,0.3f });
	fallingRange_->GetDirectionalLight()->SetEnableLighting(false);

	// チュートリアル
	guideBlockWorld_.Initialize();
	for (int i = 0; i < kBlockNumX; i++) {
		if (i != 6) {
			guideBlock_[i] = new Block();
			guideBlockWorld_.translation_ = { (2 * i) - (float)(kBlockNumX - 1) , -5, 0 };
			guideBlock_[i]->Initialize(guideBlockWorld_, BlockTexHandle_, model_.get());
			blocks_.push_back(guideBlock_[i]);
			collisionManager_->SetColliderList(guideBlock_[i]);
		}
		else {
			guideBlock_[i] = new Block();
			guideBlockWorld_.translation_ = { (2 * i) - (float)(kBlockNumX - 1) , 15, 0 };
			guideBlock_[i]->Initialize(guideBlockWorld_, BlockTexHandle_, model_.get());
			blocks_.push_back(guideBlock_[i]);
			collisionManager_->SetColliderList(guideBlock_[i]);
		}
	}

	worldTransform_.UpdateMatrix();
	for (int i = 0; i < 2; i++) {
		wallWorld_[i].UpdateMatrix();
	}
	floorWorld_.UpdateMatrix();

	//for (int i = 0; i < 4; i++) {
	//	NextworldTransform_[i].UpdateMatrix();
	//}
}

void BlockManager::Update() {
	worldTransform_.UpdateMatrix();
	for (int i = 0; i < 2; i++) {
		wallWorld_[i].UpdateMatrix();
	}
	floorWorld_.UpdateMatrix();

	for (int i = 0; i < 4; i++) {
		NextworldTransform_[i].UpdateMatrix();
	}

#pragma region 入力処理
	if (input_->IsPushKeyEnter(DIK_RIGHT) || gamePad_->TriggerButton(XINPUT_GAMEPAD_DPAD_RIGHT)) {
		for (int i = 0; i < 4; i++) {
			if (NextworldTransform_[i].translation_.x <= wallWorld_[1].translation_.x - 2) {
				isNextBlockInMap_ = true;
			}
			else {
				isNextBlockInMap_ = false;
				break;
			}
		}
		if (isNextBlockInMap_) {
			worldTransform_.translation_.x += 2.00f;
			for (int i = 0; i < 4; i++) {
				NextworldTransform_[i].translation_.x += 2.00f;
			}
		}
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT) || gamePad_->TriggerButton(XINPUT_GAMEPAD_DPAD_LEFT)) {
		for (int i = 0; i < 4; i++) {
			if (NextworldTransform_[i].translation_.x >= wallWorld_[0].translation_.x + 2) {
				isNextBlockInMap_ = true;
			}
			else {
				isNextBlockInMap_ = false;
				break;
			}
		}
		if (isNextBlockInMap_) {
			worldTransform_.translation_.x -= 2.00f;
			for (int i = 0; i < 4; i++) {
				NextworldTransform_[i].translation_.x -= 2.00f;
			}
		}
	}
	if (input_->IsPushKeyEnter(DIK_SPACE) || gamePad_->TriggerButton(XINPUT_GAMEPAD_A)) {
		audio_->SoundPlayWave(soundHandle_[1], false,1.5f);
		if (isDropBlock_) {
			//形状をランダムにする
			ChangeShape_[0] = ChangeShape_[1];
			ChangeShape_[1] = ChangeShape_[2];
			ChangeShape_[2] = Shape(rand() % 8);

			//２つのブロックの確率変動
			index_ = Changeindex_[0];
			Changeindex_[0] = Changeindex_[1];
			Changeindex_[1] = Changeindex_[2];
			Changeindex_[2] = rand() % 10;
			ShapeManagement();
			isDropBlock_ = false;
		}
	}
	shape_ = ChangeShape_[0];
#pragma endregion
	if (!isDropBlock_) {
		if (dropCoolTime_ >= 0) {
			dropCoolTime_--;
			if (dropCoolTime_ <= 0) {
				isDropBlock_ = true;
				dropCoolTime_ = kDropCoolTime;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		int count = 1;
		if (i == 0) {
			fallingPoint_[i].translation_.x = NextworldTransform_[i].translation_.x;
		}
		else {
			for (int j = 0; j < 4; j++) {
				if (NextworldTransform_[i].translation_.x != NextworldTransform_[i - count].translation_.x) {
					fallingPoint_[i].translation_.x = NextworldTransform_[i].translation_.x;

				}
				else {
					fallingPoint_[i].translation_.x = -100;
					break;
				}
				if (i - count != 0) {
					count++;
				}
				else {
					break;
				}
			}
		}
	}

	for (Block* block_ : blocks_) {
		block_->Update();
	}

	for (int i = 0; i < 4; i++) {
		fallingPoint_[i].UpdateMatrix();
	}

	// ブロックが横一列になっていたら消す
	CheckAndClearRow();

#ifdef _DEBUG
	ImGui::Begin("shape");
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &NextworldTransform_[0].translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &NextworldTransform_[0].rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &NextworldTransform_[0].scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &NextworldTransform_[1].translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &NextworldTransform_[1].rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &NextworldTransform_[1].scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &NextworldTransform_[2].translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &NextworldTransform_[2].rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &NextworldTransform_[2].scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("worldTransform")) {
		ImGui::DragFloat3("translate", &NextworldTransform_[3].translation_.x, 0.1f, 100, 100);
		ImGui::DragFloat3("rotate", &NextworldTransform_[3].rotation_.x, 0.01f, -6.28f, 6.28f);
		ImGui::DragFloat3("scale", &NextworldTransform_[3].scale_.x, 0.01f, 0, 10);
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG
}

void BlockManager::Draw(ViewProjection viewProjection_) {
	Shape_one(viewProjection_);
	// ブロック
	for (Block* block_ : blocks_) {
		block_->Draw(viewProjection_);
	}
	//for (HeadBlock* headblock_ : headblocks_) {
	//	headblock_->Draw(viewProjection_);
	//}
	// 壁
	for (int i = 0; i < 2; i++) {
		wall_[i]->Draw(wallWorld_[i], viewProjection_);
	}
	// 床
	floor_->Draw(floorWorld_, viewProjection_);
	// 落下予測地点
	if (isFallingPoint_) {
		for (int i = 0; i < 4; i++) {
			fallingRange_->Draw(fallingPoint_[i], viewProjection_, fallingRangeTexHandle_);
		}
	}
}

void BlockManager::Shape_one(ViewProjection viewProjection_) {
	switch (shape_)
	{
	case Shape::shape_I:
#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width * 2;
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width * 2;
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 3;
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 3;
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;

	case Shape::shape_T:
#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;

	case Shape::shape_S:
#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;

	case Shape::shape_O:

#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;

	case Shape::shape_J:
#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x - width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}

#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 2;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 2;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;

	case Shape::shape_L:
#pragma region ブロックの１番
		if (Changeindex_[0] != 0) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] != 1) {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}

#pragma endregion

#pragma region ブロックの３番
		if (Changeindex_[0] != 2) {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[2].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + width;
			Nextmodel_[2]->Draw(NextworldTransform_[2], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion

#pragma region ブロックの４番
		if (Changeindex_[0] != 3) {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 2;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[3].translation_.x = worldTransform_.translation_.x;
			NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + width * 2;
			Nextmodel_[3]->Draw(NextworldTransform_[3], viewProjection_, hardBlockTexHandle_);
		}
#pragma endregion
		break;
	case Shape::shape_ten:
#pragma region ブロックの１番
		NextworldTransform_[1].translation_ = worldTransform_.translation_;
		NextworldTransform_[2].translation_ = worldTransform_.translation_;
		NextworldTransform_[3].translation_ = worldTransform_.translation_;
		NextworldTransform_[1].translation_.y = worldTransform_.translation_.y + 10;
		NextworldTransform_[2].translation_.y = worldTransform_.translation_.y + 10;
		NextworldTransform_[3].translation_.y = worldTransform_.translation_.y + 10;

		if (Changeindex_[0] == 0 || Changeindex_[0] == 1) {
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}

#pragma endregion

		break;
	case Shape::shape_side:
#pragma region ブロックの１番

		NextworldTransform_[2].translation_ = worldTransform_.translation_;
		NextworldTransform_[3].translation_ = worldTransform_.translation_;

		if (Changeindex_[0] == 0 || Changeindex_[0] == 1) {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[0].translation_ = worldTransform_.translation_;
			Nextmodel_[0]->Draw(NextworldTransform_[0], viewProjection_, hardBlockTexHandle_);
		}

#pragma endregion

#pragma region ブロックの２番
		if (Changeindex_[0] == 2 || Changeindex_[0] == 3) {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, BlockTexHandle_);
		}
		else {
			NextworldTransform_[1].translation_.x = worldTransform_.translation_.x + width;
			NextworldTransform_[1].translation_.y = worldTransform_.translation_.y;
			Nextmodel_[1]->Draw(NextworldTransform_[1], viewProjection_, hardBlockTexHandle_);
		}

#pragma endregion


		break;
	}
}

void BlockManager::Shape_Second(ViewProjection viewProjection_) {
	///画像で予測ブロックを表示する（次のブロック）
	shape_ = ChangeShape_[1];
	switch (shape_) {
	case Shape::shape_I:
		break;
	case Shape::shape_T:
		break;
	case Shape::shape_S:
		break;
	case Shape::shape_O:
		break;
	case Shape::shape_J:
		break;
	case Shape::shape_L:
		break;
	case Shape::shape_ten:
		break;
	case Shape::shape_side:
		break;
	}
}

void BlockManager::ShapeManagement() {
	switch (shape_)
	{
	case Shape::shape_I:
		Shape_I(worldTransform_.translation_, index_);
		break;

	case Shape::shape_T:
		Shape_T(worldTransform_.translation_, index_);
		break;

	case Shape::shape_S:
		Shape_S(worldTransform_.translation_, index_);
		break;

	case Shape::shape_O:
		Shape_O(worldTransform_.translation_, index_);
		break;

	case Shape::shape_J:
		Shape_J(worldTransform_.translation_, index_);
		break;

	case Shape::shape_L:
		Shape_L(worldTransform_.translation_, index_);
		break;
	case Shape::shape_ten:
		Shape_Ten(worldTransform_.translation_, index_);
		break;
	case Shape::shape_side:
		shape_side(worldTransform_.translation_, index_);
		break;
	}
}

/// <summary>
/// I字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_I(Vector3 velocity, int index) {

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
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x,velocity.y + height * 2,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x,velocity.y + height * 2,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x,velocity.y + height * 3,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x,velocity.y + height * 3 ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
#pragma endregion
}

/// <summary>
/// T字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_T(Vector3 velocity, int index) {


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
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x + width,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x + width,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x - width,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x - width,velocity.y + height  ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
#pragma endregion
}

/// <summary>
/// S字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_S(Vector3 velocity, int index) {
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
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
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
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x - width,velocity.y ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + height  ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
#pragma endregion
}

/// <summary>
/// O字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_O(Vector3 velocity, int index) {
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
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
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
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x + width,velocity.y + height  ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
}

/// <summary>
/// J字ブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_J(Vector3 velocity, int index) {
#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x - width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x - width,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + height * 2,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + height * 2 ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
}

/// <summary>
///L字ブロック 
/// </summary>
/// <param name="velocity"></param>
void BlockManager::Shape_L(Vector3 velocity, int index) {
#pragma region ブロックの１番
	if (index != 0) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x + width,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index != 1) {
		// 実体生成
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		//当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion

#pragma region ブロックの３番
	if (index != 2) {
		// 実体生成
		Block* newBlock_3 = new Block();
		// 初期化
		newBlock_3->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		blocks_.push_back(newBlock_3);
		collisionManager_->SetColliderList(newBlock_3);
	}
	else {
		Block* newBlock_3 = new Block();
		newBlock_3->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_3->SetIsHardBlock(true);
		newBlock_3->SetworldTransform_({ velocity.x ,velocity.y + height,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_3);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_3);
	}
#pragma endregion

#pragma region ブロックの４番
	if (index != 3) {
		// 実体生成
		Block* newBlock_4 = new Block();
		// 初期化
		newBlock_4->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + height * 2,velocity.z });
		blocks_.push_back(newBlock_4);
		collisionManager_->SetColliderList(newBlock_4);
	}
	else {
		Block* newBlock_4 = new Block();
		newBlock_4->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_4->SetIsHardBlock(true);
		newBlock_4->SetworldTransform_({ velocity.x ,velocity.y + height * 2 ,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_4);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_4);
	}
}

/// <summary>
/// 
/// </summary>
/// /// <param name="velocity"></param>
void BlockManager::Shape_Ten(Vector3 velocity, int index) {
#pragma region ブロックの１番
	if (index == 0 || index == 1) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion
}

/// <summary>
/// 横に連なるブロック
/// </summary>
/// <param name="velocity"></param>
void BlockManager::shape_side(Vector3 velocity, int index) {
#pragma region ブロックの１番
	if (index == 0 || index == 1) {
		Block* newBlock_1 = new Block();
		// 初期化
		newBlock_1->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_1);
		collisionManager_->SetColliderList(newBlock_1);
	}
	else {
		Block* newBlock_1 = new Block();
		newBlock_1->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_1->SetIsHardBlock(true);
		newBlock_1->SetworldTransform_({ velocity.x,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_1);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_1);
	}
#pragma endregion

#pragma region ブロックの２番
	if (index == 2 || index == 3) {
		Block* newBlock_2 = new Block();
		// 初期化
		newBlock_2->Initialize(worldTransform_, BlockTexHandle_, model_.get());
		newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y ,velocity.z });
		blocks_.push_back(newBlock_2);
		collisionManager_->SetColliderList(newBlock_2);
	}
	else {
		Block* newBlock_2 = new Block();
		newBlock_2->Initialize(worldTransform_, hardBlockTexHandle_, hardBlockModel_.get());
		newBlock_2->SetIsHardBlock(true);
		newBlock_2->SetworldTransform_({ velocity.x + width,velocity.y,velocity.z });
		//リストに登録
		blocks_.push_back(newBlock_2);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_2);
	}
#pragma endregion
}

void BlockManager::CheckAndClearRow() {
	for (int i = 0; i < kBlockNumY; i++) {
		// その列にブロックがいくつあるかの確認
		int count = 0;
		int hardBlockCount = 0;
		// 落下するブロック
		for (Block* block : blocks_) {
			if (!block->GetFoolFlag()) {
				if ((int)clearBlock_[i].y == static_cast<int>(std::round(block->GetWorldPosition().y))) {
					for (int j = 0; j < kBlockNumX; j++) {
						if ((int)clearBlock_[j].x == static_cast<int>(std::round(block->GetWorldPosition().x))) {
							count++;
							if (!block->GetIsHardBlock()) {
								block->SetIsAlive(false);
							}
							else if (block->GetIsHardBlock()) {
								hardBlockCount++;
							}
						}
					}
				}
			}

		}
		//// 消えないブロック
		//for (HeadBlock* headBlock : headblocks_) {
		//	if (!headBlock->GetFoolFlag()) {
		//		if ((int)clearBlock_[i].y == static_cast<int>(std::round(headBlock->GetWorldPosition().y))) {
		//			for (int j = 0; j < kBlockNumX; j++) {
		//				if ((int)clearBlock_[j].x == static_cast<int>(std::round(headBlock->GetWorldPosition().x))) {
		//					count++;
		//				}
		//			}
		//		}
		//	}
		//}


		if (count >= kBlockNumX) {
			if (hardBlockCount <= kBlockNumX - 1) {
				/*for (Block* block : blocks_) {
					if (!block->GetIsAlive() && !block->GetIsHardBlock()) {
						block->SetIsDelete(true);
					}
				}*/
				//collisionManager_->CheckDeleteColliderList();
				blocks_.remove_if([](Block* block) {
					if (!block->GetIsAlive()) {
						delete block;
						return true;
					}
					return false;
					});
				;

				// コライダーをすべてクリア
				collisionManager_->ClearColliderList();
				AABB aabb = {
					{-0.99999f,-1.0f,-0.99999f},
					{0.99999f,1.0f,0.99999f}
				};
				// すでに生成されているブロックをコライダーに登録
				// 落下するブロック
				for (Block* block : blocks_) {
					// 当たり判定の形状を設定
					block->SetCollisionPrimitive(kCollisionPrimitiveAABB);
					block->SetCollisionAttribute(kCollisionAttributeBlock);
					block->SetAABB(aabb);
					collisionManager_->SetColliderList(block);
				}
				// コライダーのすべてが初期化されてしまっているのでplayerを再pushする
				isDelete_ = true;
			}
		}
		else {
			for (Block* block : blocks_) {
				block->SetIsAlive(true);
			}
		}
	}
}

void BlockManager::DeleteBlocksAboveGoalLine() {
	for (Block* block : blocks_) {
		if (!block->GetFoolFlag()) {
			if (goalLinePos_.y <= block->GetWorldPosition().y || block->GetWorldPosition().x >= wallWorld_[1].translation_.x || block->GetWorldPosition().x <= wallWorld_[0].translation_.x) {
				block->SetIsAlive(false);
				// コライダーのすべてが初期化されてしまっているのでplayerを再pushする
				isDelete_ = true;
			}
		}
	}
	if (isDelete_) {
		audio_->SoundPlayWave(soundHandle_[0], false,1.5f);
		blocks_.remove_if([](Block* block) {
			if (!block->GetIsAlive()) {
				delete block;
				return true;
			}
			return false;
			});
		;

		// コライダーをすべてクリア
		collisionManager_->ClearColliderList();
		AABB aabb = {
			{-0.99999f,-1.0f,-0.99999f},
			{0.99999f,1.0f,0.99999f}
		};
		// すでに生成されているブロックをコライダーに登録
		// 落下するブロック
		for (Block* block : blocks_) {
			// 当たり判定の形状を設定
			block->SetCollisionPrimitive(kCollisionPrimitiveAABB);
			block->SetCollisionAttribute(kCollisionAttributeBlock);
			block->SetAABB(aabb);
			collisionManager_->SetColliderList(block);
		}
	}
}

void BlockManager::GuideDeleteBlock() {
	for (Block* block_ : blocks_) {
		block_->Update();
	}

	// ブロックが横一列になっていたら消す
	CheckAndClearRow();
}