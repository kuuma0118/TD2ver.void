#include "GameScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {
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

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0.0f,0.0f,-50.0f };
	worldTransform_.translation_.y = 5.0f;
	
};

void GameScene::Update(GameManager* gameManager) {
	worldTransform_.UpdateMatrix();
	viewProjection_.UpdateMatrix();

	for (Block* block_ : blocks_) {
		block_->Update();
	}

	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		worldTransform_.translation_.x += 2.0f;
	}
	else if (input_->IsPushKeyEnter(DIK_LEFT)) {
		worldTransform_.translation_.x -= 2.0f;
	}

	if (input_->IsPushKeyEnter(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, kBulletSpeed, 0);
		Block* newBlock_ = new Block();

		newBlock_->Initialize(worldTransform_);
		blocks_.push_back(newBlock_);

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