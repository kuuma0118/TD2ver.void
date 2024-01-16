#include "SelectScene.h"
#include "GameManager.h"
#include "GameScene.h"
#include <algorithm>

GameTitleScene::GameTitleScene() {};

GameTitleScene::~GameTitleScene() {};

void GameTitleScene::Initialize(GameManager* gameManager)
{
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();

	soundHandle_ = audio_->SoundLoadWave("Resources/Sounds/Selection.wav");

	//スプライトの生成
	transitionSprite_.reset(Sprite::Create(transitionTextureHandle_, { 0.0f,0.0f }));
	transitionSprite_->SetColor(transitionColor_);
	transitionSprite_->SetSize(Vector2{ 640.0f,360.0f });

	viewProjection_.UpdateMatrix();

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();
};

void GameTitleScene::Update(GameManager* gameManager)
{
	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (isTransitionEnd_) {
			isTransition_ = true;
		}
	}

	//if (input_->IsPushKeyEnter(DIK_SPACE))
	//{
	//	if (isTransitionEnd_) {
	//		isTransition_ = true;
	//		if (soundCount_ == 0)
	//		{
	//			soundCount_ = 1;
	//			audio_->SoundPlayWave(soundHandle_, false);
	//		}
	//	}
	//}

	if (isTransitionEnd_ == false) {
		transitionTimer_ += 1.0f / kTransitionTime;
		transitionColor_.w = Lerp(transitionColor_.w, 0.0f, transitionTimer_);
		transitionSprite_->SetColor(transitionColor_);

		if (transitionColor_.w <= 0.0f) {
			isTransitionEnd_ = true;
			transitionTimer_ = 0.0f;
		}
	}

	//if (isTransition_) {
	//	transitionTimer_ += 1.0f / kTransitionTime;
	//	transitionColor_.w = Lerp(transitionColor_.w, 1.0f, transitionTimer_);
	//	transitionSprite_->SetColor(transitionColor_);

	//	if (transitionColor_.w >= 1.0f) {
	//		gameManager->ChangeScene(new GameScene);
	//	}
	//}

	viewProjection_.UpdateMatrix();

};

void GameTitleScene::Draw(GameManager* gameManager)
{
	PostProcess::GetInstance()->PreDraw();

#pragma region 背景スプライトの描画

	//背景スプライトの描画
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion

	//深度バッファをクリア
	FCS::GetInstance()->ClearDepthBuffer();

#pragma region モデルの描画

	//モデルの描画
	Model::PreDraw();

	Model::PostDraw();

#pragma endregion

#pragma region パーティクルの描画

	//パーティクルモデルの描画
	ParticleModel::PreDraw();

	ParticleModel::PostDraw();

#pragma endregion

	PostProcess::GetInstance()->PostDraw();

#pragma region 前景スプライトの描画

	//スプライトの描画処理
	Sprite::PreDraw(Sprite::kBlendModeNormal);

	//transitionSprite_->Draw();

	Sprite::PostDraw();

#pragma endregion

};