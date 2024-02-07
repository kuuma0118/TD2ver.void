#include "GameScene.h"
#include "GameManager.h"
#include "ClearScene.h"
#include "OverScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
	//delete player_;
	//delete blockManager_;
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
	player_ = std::make_unique<Player>();
	player_->Initialize();

	dropUITextureHandle_ = TextureManager::Load("Resources/Pictures/dropUI.png");
	dropUISprite_.reset(Sprite::Create(dropUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 10.0f * 0.5f , 55.0f }));

	moovUITextureHandle_ = TextureManager::Load("Resources/Pictures/moovUI.png");
	moovUISprite_.reset(Sprite::Create(moovUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 10.0f * 0.5f , 0.0f }));

	// 当たり判定のインスタンスを生成
	collisionManager_ = std::make_unique<CollisionManager>();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_.get());

	// ブロックマネージャ
	blockManager_ = std::make_unique<BlockManager>();
	blockManager_->Initialize(collisionManager_.get());
	// ブロックの発生位置
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 5.0f;
	worldTransform_.UpdateMatrix();

	// カメラ
	viewProjection_.Initialize();

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	isOpeningCamera_ = true;
	currentFrame_ = 0;

	// ゴールライン
	goalLine_ = std::make_unique<GoalLine>();
	goalLine_->Initialize();
	goalLine_->SetPlayer(player_.get());

	// デッドライン
	deadLine_ = std::make_unique<DeadLine>();
	deadLine_->Initialize();
	deadLine_->SetPlayer(player_.get());
	deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());

	// カメラをセット
	followCamera_->SetTarget(&player_->GetWorldTransform());
	// ゴールラインをセット
	blockManager_->SetGoalLinePos(goalLine_->GetWorldPosition());
	isOpeningCamera_ = true;
};

void GameScene::Update(GameManager* gameManager) {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	// カメラ
	followCamera_->Update();
	viewProjection_.matView_ = followCamera_->GetViewProjection().matView_;
	viewProjection_.matProjection_ = followCamera_->GetViewProjection().matProjection_;
	viewProjection_.TransferMatrix();

	// チュートリアル用のカメラ
	if (isOpeningCamera_) {
		if (currentFrame_ >= 120 && currentFrame_ <= 300) {
			followCamera_->SetNextTarget(&goalLine_->GetWorldTransform());
		}
		else if (currentFrame_ >= 301 && currentFrame_ <= 480) {
			followCamera_->SetNextTarget(&deadLine_->GetWorldTransform());
		}
		else if (currentFrame_ >= 481 && currentFrame_ <= 530) {
			followCamera_->SetNextTarget(nullptr);
		}
		else if (currentFrame_ >= 531 && currentFrame_ <= 650) {
			blockManager_->GuideDeleteBlock();
			// デッドライン
			deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());
			deadLine_->Update(viewProjection_);
		}
		else if(currentFrame_ >= 651){
			currentFrame_ = 0;
			isOpeningCamera_ = false;
		}
		currentFrame_++;
	}
	// ゲーム開始
	else {
		// 自機
		player_->Update();
		// ブロックマネージャ
		blockManager_->Update();
		// ゴールライン
		goalLine_->Update(viewProjection_);
		// デッドライン
		deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());
		deadLine_->Update(viewProjection_);
	}

	// ゴールラインより上にあるブロックを消す
	blockManager_->DeleteBlocksAboveGoalLine();
	// ブロックが消えていた場合
	if (blockManager_->GetIsDelete()) {
		AABB aabb = {
			{-0.8f,-1.0f,-0.8f},
			{0.8f,1.0f,0.8f}
		};
		player_->SetAABB(aabb);
		// 自機をコライダーにセット
		collisionManager_->SetColliderList(player_.get());
		player_->SetCollisionAttribute(kCollisionAttributePlayer);
		player_->SetCollisionPrimitive(kCollisionPrimitiveAABB);
		// ブロックの消えるフラグをfalse
		blockManager_->SetIsDelete(false);
	}
	// 当たり判定
	collisionManager_->CheckAllCollisions();

	// 自機が死んだらゲームオーバー
	if (!player_->GetIsAlive()) {
		gameManager->ChangeScene(new GameOverScene);
	}
	// ゴールラインに達したらクリア
	else if (goalLine_->GetIsGoal()) {
		gameManager->ChangeScene(new GameClearScene);
	}

#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("translation", &viewProjection_.translation_.x, 0.001f, -100, 100);
	ImGui::Checkbox("OpeningCamera", &isOpeningCamera_);
	ImGui::End();
#endif
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

	dropUISprite_->Draw();

	moovUISprite_->Draw();

	// ゴールライン
	//goalLine_->Draw2DLine();

	// デッドライン
	//deadLine_->Draw2DLine();

	Sprite::PostDraw();

#pragma endregion
};