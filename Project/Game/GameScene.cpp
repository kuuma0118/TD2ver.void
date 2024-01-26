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
	worldTransform_.UpdateMatrix();
	// 自機
	player_ = new Player();
	player_->Initialize();

	// 当たり判定のインスタンスを生成
	collisionManager_ = new CollisionManager();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_);

	// ブロックマネージャ
	blockManager_ = new BlockManager();
	blockManager_->Initialize(collisionManager_);
	// ブロックの発生位置
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 5.0f;
	worldTransform_.UpdateMatrix();

	// カメラ
	viewProjection_.Initialize();
	//viewProjection_.translation_ = { 0.0f,5.0f,-50.0f };
	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	// ゴールライン
	goalLine_ = std::make_unique<GoalLine>();
	goalLine_->Initialize();
	goalLine_->SetPlayer(player_);

	// デッドライン
	deadLine_ = std::make_unique<DeadLine>();
	deadLine_->Initialize();
	deadLine_->SetPlayer(player_);
	deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());
};

void GameScene::Update(GameManager* gameManager) {
	worldTransform_.UpdateMatrix();

	viewProjection_.UpdateMatrix();
	// カメラ
	followCamera_->Update();
	viewProjection_.matView_ = followCamera_->GetViewProjection().matView_;
	viewProjection_.matProjection_ = followCamera_->GetViewProjection().matProjection_;
	viewProjection_.TransferMatrix();

	
	// 自機
	player_->Update();

	blockManager_->Update();

	// ゴールライン
	goalLine_->Update(followCamera_->GetViewProjection());

	// デッドライン
	deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());
	deadLine_->Update(followCamera_->GetViewProjection());

	// ブロックが消えていた場合
	if (blockManager_->GetIsDelete()) {
		// 自機をコライダーにセット
		collisionManager_->SetColliderList(player_);
		// ブロックの消えるフラグをfalse
		blockManager_->SetIsDelete(false);
	}
	// 当たり判定
	collisionManager_->CheckAllCollisions();

	// 自機が死んだらゲームオーバー
	if (player_->GetIsAlive()) {

	}
	// ゴールラインに達したらクリア
	if (goalLine_->GetIsGoal()) {

	}

	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.001f, -100, 100);
	ImGui::End();
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
	player_->Draw(followCamera_->GetViewProjection());

	//ブロックの描画
	blockManager_->Draw(followCamera_->GetViewProjection());

	// ゴールライン
	goalLine_->Draw3DLine(followCamera_->GetViewProjection());

	// デッドライン
	deadLine_->Draw3DLine(followCamera_->GetViewProjection());

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