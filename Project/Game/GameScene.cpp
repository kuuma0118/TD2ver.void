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
	viewProjection_.translation_ = { 0.0f,5.0f,-50.0f };
	worldTransform_.translation_.y = 5.0f;
	worldTransform_.UpdateMatrix();
	// 自機
	player_ = new Player();
	player_->Initialize();

	// ゴールライン
	goalLine_ = std::make_unique<GoalLine>();
	goalLine_->Initialize();
	goalLine_->SetPlayer(player_);

	// デッドライン
	deadLine_ = std::make_unique<DeadLine>();
	deadLine_->Initialize();
	deadLine_->SetPlayer(player_);

	// 当たり判定のインスタンスを生成
	collisionManager_ = new CollisionManager();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_);

	blockManager_ = new BlockManager();
	blockManager_->Initialize(collisionManager_);
};

void GameScene::Update(GameManager* gameManager) {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		worldTransform_.translation_.x += 2.00f;
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT)) {
		worldTransform_.translation_.x -= 2.00f;
	}
	// 自機
	player_->Update();

	blockManager_->Update(worldTransform_.translation_);

	// 当たり判定
	collisionManager_->CheckAllCollisions();

	// ゴールライン
	goalLine_->Update(viewProjection_);

	// デッドライン
	deadLine_->Update(viewProjection_);

	// 自機が死んだらゲームオーバー
	if (player_->GetIsAlive()) {

	}
	// ゴールラインに達したらクリア
	if (goalLine_->GetIsGoal()) {

	}
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

	// ゴールライン
	goalLine_->Draw3DLine(viewProjection_);

	// デッドライン
	deadLine_->Draw3DLine(viewProjection_);

	Model::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	// ゴールライン
	goalLine_->Draw2DLine();

	// デッドライン
	deadLine_->Draw2DLine();

	Sprite::PostDraw();

#pragma endregion
};