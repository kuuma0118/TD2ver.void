#include "GameScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete player_;
	for (Block* block_ : blocks_) {
		delete block_;
	}
};

void GameScene::Initialize(GameManager* gameManager) {
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	//オーディオクラスのインスタンスを取得
	audio_ = Audio::GetInstance();
	//ポストプロセスのインスタンスを取得
	postProcess_ = PostProcess::GetInstance();

	// カメラ
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-50.0f };
	worldTransform_.translation_.y = 5.0f;

	// 自機
	player_ = new Player();
	player_->Init();

	// 当たり判定のインスタンスを生成
	collisionManager_ = new CollisionManager();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_);
};

void GameScene::Update(GameManager* gameManager) {
	// 自機
	player_->Update();
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	for (Block* block_ : blocks_) {
		block_->Update();
	}

	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		worldTransform_.translation_.x += 2.001f;
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT)) {
		worldTransform_.translation_.x -= 2.001f;
	}

	if (input_->IsPushKeyEnter(DIK_SPACE)) {
		// 落下速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, kBulletSpeed, 0);
		// 実体生成
		Block* newBlock_ = new Block();
		// 初期化
		newBlock_->Initialize(worldTransform_);
		//リストに登録
		blocks_.push_back(newBlock_);
		// 当たり判定に追加
		collisionManager_->SetColliderList(newBlock_);
	}

	// 当たり判定
	collisionManager_->CheckAllCollisions();
};

void GameScene::Draw(GameManager* gameManager) {

#pragma region 背景スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion

	//深度バッファのクリア
	FCS::GetInstance()->ClearDepthBuffer();

#pragma region モデルの描画

	Model::PreDraw();

	// 自機
	player_->Draw(viewProjection_);

	//ブロックの描画
	for (Block* block_ : blocks_) {
		block_->Draw(viewProjection_);
	}

	Model::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion
};

//void GameScene::CheckAllCollisions() {
	////判定対象AとBの座標
	//Vector3 posA, posB;
	////自弾と敵弾の当たり判定
	//for (Block* block1_ : blocks_) {
	//	for (Block* block2_ : blocks2_) {
	//		//自弾の座標
	//		posA = block1_->GetworldTransform_();
	//		//敵弾の座標
	//		posB = block2_->GetworldTransform_();
	//		posA.z = 1.0f;
	//		//座標AとBの距離を求める
	//		float distance = Length(Subtract(posA, posB));

	//		

	//		//球と球の当たり判定
	//		if (distance <=13) {
	//			for (Block* block_ : blocks_) {
	//				block_->OnCollision();
 //				}
	//		}
	//	}
	//}
//}