#include "GameScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	delete player_;
	delete blockManager_;
	
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
	worldTransform_.UpdateMatrix();
	// 自機
	player_ = new Player();
	player_->Init();

	// 当たり判定のインスタンスを生成
	collisionManager_ = new CollisionManager();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_);

	blockManager_ = new BlockManager();
	blockManager_->Initialize(collisionManager_);
};

void GameScene::Update(GameManager* gameManager) {
	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		worldTransform_.translation_.x += 2.001f;
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT)) {
		worldTransform_.translation_.x -= 2.001f;
	}
	// 自機
	player_->Update();
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	blockManager_->Update(worldTransform_.translation_);


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
	blockManager_->Draw(viewProjection_);

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