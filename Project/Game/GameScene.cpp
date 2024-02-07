#include "GameScene.h"
#include "GameManager.h"
#include "ClearScene.h"
#include "OverScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {

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

	SceneSoundHandle_ = audio_->SoundLoadWave("Resources/Sounds/GameScene.wav");

	audio_->SoundPlayWave(SceneSoundHandle_, true, 0.5f);

	dropUITextureHandle_ = TextureManager::Load("Resources/Pictures/dropUI.png");
	dropUISprite_.reset(Sprite::Create(dropUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f + 500.0f * 0.5f , 70.0f }));

	moovUITextureHandle_ = TextureManager::Load("Resources/Pictures/moovUI.png");
	moovUISprite_.reset(Sprite::Create(moovUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f + 610.0f * 0.5f , 0.0f }));

	// チュートリアル用の文字
	guideGoalLineTexture_ = TextureManager::Load("Resources/Guide/Guide_Climb.png");
	guideGoalLineSprite_.reset(Sprite::Create(guideGoalLineTexture_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 10.0f * 0.5f , 55.0f }));

	guideDeadLineTexture_ = TextureManager::Load("Resources/Guide/Guide_away.png");
	guideDeadLineSprite_.reset(Sprite::Create(guideDeadLineTexture_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 10.0f * 0.5f , 120.0f }));

	guideDeleteTexture_ = TextureManager::Load("Resources/Guide/Guide_block.png");
	guideDeleteBlockSprite_.reset(Sprite::Create(guideDeleteTexture_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 480.0f * 0.5f , 320.0f }));

	guideStartTexture_ = TextureManager::Load("Resources/Guide/Guide_Start.png");
	guideStartSprite_.reset(Sprite::Create(guideStartTexture_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f , 50.0f }));

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

	// 色設定
	color_ = { 1,1,1,kMinAlpha_ };
	// デッドラインの点滅に使用するアルファ値設定
	alpha_ = 0.0f;
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
			isGuideDelete_ = false;
			isGuideDeadLine_ = false;
			isGuideGoalLine_ = true;
		}
		else if (currentFrame_ >= 301 && currentFrame_ <= 480) {
			followCamera_->SetNextTarget(&deadLine_->GetWorldTransform());
			isGuideGoalLine_ = false;
			isGuideDelete_ = false;
			isGuideDeadLine_ = true;
		}
		else if (currentFrame_ >= 481 && currentFrame_ <= 530) {
			followCamera_->SetNextTarget(nullptr);
			isGuideGoalLine_ = false;
			isGuideDeadLine_ = false;
			isGuideDelete_ = true;
		}
		else if (currentFrame_ >= 531 && currentFrame_ <= 700) {
			blockManager_->GuideDeleteBlock();
			// デッドライン
			deadLine_->SetIsBlockDelete(blockManager_->GetIsDelete());
			deadLine_->Update(viewProjection_);
		}
		else if(currentFrame_ >= 701){
			isGuideStart_ = true;
			currentFrame_ = 0;
			isOpeningCamera_ = false;
		}
		currentFrame_++;
	}
	// ゲーム開始
	else {
		if (isGuideStart_) {
			guideClearFrame_++;
			// alphaが上限値までいったら0にする
			if (alpha_ >= kMaxAlpha_) {
				alpha_ = 0.0f;
			}
			// デッドラインを点滅
			alpha_ += kFlashSpeed_;
			color_.w = kMinAlpha_ + sinf(alpha_) * (kMaxAlpha_ - kMinAlpha_);
			// アルファ値を反映
			guideStartSprite_->SetColor(color_);

			if (guideClearFrame_ >= 60) {
				isGuideStart_ = false;
			}
		}

		isGuideDelete_ = false;
		isGuideDeadLine_ = false;
		isGuideGoalLine_ = false;
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
		audio_->StopAudio(SceneSoundHandle_);
		gameManager->ChangeScene(new GameOverScene);
	}
	// ゴールラインに達したらクリア
	else if (goalLine_->GetIsGoal()) {
		audio_->StopAudio(SceneSoundHandle_);
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

	PostProcess::GetInstance()->PreDraw();

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

	PostProcess::GetInstance()->PostDraw();

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);
	if (!isOpeningCamera_) {
		dropUISprite_->Draw();

		moovUISprite_->Draw();
	}

	if (isGuideGoalLine_) {
		guideGoalLineSprite_->Draw();
	}
	else if(isGuideDeadLine_) {
		guideDeadLineSprite_->Draw();
	}
	else if (isGuideDelete_) {
		guideDeleteBlockSprite_->Draw();
	}
	if (isGuideStart_) {
		guideStartSprite_->Draw();
	}

	Sprite::PostDraw();

#pragma endregion
};