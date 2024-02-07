#include "OverScene.h"
#include "GameManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameOverScene::GameOverScene() {};

GameOverScene::~GameOverScene() {
	//delete player_;
	//for (Block* block_ : blocks_) {
	//	delete block_;
	//}
};

void GameOverScene::Initialize(GameManager* gameManager) {

	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	//オーディオクラスのインスタンスを取得
	audio_ = Audio::GetInstance();
	//ポストプロセスのインスタンスを取得
	postProcess_ = PostProcess::GetInstance();

	overSoundHandle_ = audio_->SoundLoadWave("Resources/Sounds/over.wav");

	audio_->SoundPlayWave(overSoundHandle_, true);

	// カメラ
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,5.0f,-50.0f };
	worldTransform_.translation_.y = 5.0f;

	// 自機
	player_ = new Player();
	player_->Initialize();

	//タイトル
	overTextureHandle_ = TextureManager::Load("Resources/Pictures/over.png");
	overSprite_.reset(Sprite::Create(overTextureHandle_, { 0.0f,0.0f }));
	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });

	RestartUITextureHandle_ = TextureManager::Load("Resources/Pictures/RestartUI.png");
	RestartUISprite_.reset(Sprite::Create(RestartUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 550.0f * 0.5f , 500.0f }));

	ReturnTitleUITextureHandle_ = TextureManager::Load("Resources/Pictures/ReturnTitleUI.png");
	ReturnTitleUISprite_.reset(Sprite::Create(ReturnTitleUITextureHandle_,
		{ WinApp::GetInstance()->kClientWidth * 0.5f - 490.0f * 0.5f , 570.0f }));

	// 当たり判定のインスタンスを生成
	collisionManager_ = new CollisionManager();
	// ゲームオブジェクトをコライダーのリストに登録
	collisionManager_->SetColliderList(player_);
};

void GameOverScene::Update(GameManager* gameManager) {
	//// 自機が死んだらシーンを切り替える
	//if (player_->GetIsAlive()) {

	//}
	//// 自機
	//player_->Update();
	//worldTransform_.UpdateMatrix();
	//viewProjection_.UpdateMatrix();

	//for (Block* block_ : blocks_) {
	//	block_->Update();
	//}

	//if (input_->IsPushKeyEnter(DIK_RIGHT)) {
	//	worldTransform_.translation_.x += 2.00f;
	//}
	//else if (input_->IsPushKeyEnter(DIK_LEFT)) {
	//	worldTransform_.translation_.x -= 2.00f;
	//}

	//if (input_->IsPushKeyEnter(DIK_SPACE)) {
	//	// 落下速度
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, kBulletSpeed, 0);
	//	// 実体生成
	//	Block* newBlock_ = new Block();
	//	// 初期化
	//	newBlock_->Initialize(worldTransform_);
	//	//リストに登録
	//	blocks_.push_back(newBlock_);
	//	// 当たり判定に追加
	//	collisionManager_->SetColliderList(newBlock_);
	//}

	if (input_->IsPushKeyEnter(DIK_T) || GamePad::GetInstance()->TriggerButton(XINPUT_GAMEPAD_A))
	{
		if (!isTransition4Game_) {
			isTransition4Title_ = true;
		}
		if (isTransitionEnd_) {
			isTransition_ = true;
			if (soundCount_ == 0)
			{
				soundCount_ = 1;
				/*audio_->SoundPlayWave(SelectsoundHandle_, false);*/
			}
		}
	}
	else if (input_->IsPushKeyEnter(DIK_G) || GamePad::GetInstance()->TriggerButton(XINPUT_GAMEPAD_B)){
		if (!isTransition4Title_) {
			isTransition4Game_ = true;
		}
		if (isTransitionEnd_) {
			isTransition_ = true;
			if (soundCount_ == 0)
			{
				soundCount_ = 1;
				/*audio_->SoundPlayWave(SelectsoundHandle_, false);*/
			}
		}
	}

	if (isTransitionEnd_ == false) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 0.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w <= 0.0f) {
			isTransitionEnd_ = true;
			transitionTimer_ = 0.0f;
		}
	}

	if (isTransition_) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 1.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w >= 1.0f) {
			audio_->StopAudio(overSoundHandle_);
			if (isTransition4Title_) {
				gameManager->ChangeScene(new GameTitleScene);
			}
			else if(isTransition4Game_) {
				gameManager->ChangeScene(new GameScene);

			}
		}
	}
};

void GameOverScene::Draw(GameManager* gameManager) {

#pragma region 背景スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion

	//深度バッファのクリア
	FCS::GetInstance()->ClearDepthBuffer();

#pragma region モデルの描画

	Model::PreDraw();

	Model::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	overSprite_->Draw();

	RestartUISprite_->Draw();

	ReturnTitleUISprite_->Draw();

	transitionSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion
};