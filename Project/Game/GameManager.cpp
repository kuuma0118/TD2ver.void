#include "GameManager.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Engine/Utility/GlobalVariables.h"

GameManager::GameManager() {
	//ウィンドウズアプリケーションの初期化
	winApp_ = WinApp::GetInstance();
	winApp_->CreateGameWindow(L"DirectXGame", WinApp::kClientWidth, WinApp::kClientHeight);

	//DirectXの初期化
	dxCommon_ = FCS::GetInstance();
	dxCommon_->Initialize();

	//TextureManagerの初期化
	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize();

	//ImGuiの初期化
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize();

	//入力クラスの初期化
	input_ = Input::GetInstance();
	input_->Initialize();

	gamePad_ = GamePad::GetInstance();

	//オーディオクラスの初期化
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//ポストプロセスの初期化
	postProcess_ = PostProcess::GetInstance();
	postProcess_->Initialize();

	//モデルの静的初期化
	Model::StaticInitialize();

	//スプライトの静的初期化
	Sprite::StaticInitialize();

	//グローバル変数の読み込み
	//GlobalVariables::GetInstance()->LoadFiles();

	//シーンの初期化
	currentScene_ = new GameTitleScene();
	currentScene_->Initialize(this);
}

GameManager::~GameManager() {
	//シーンの削除
	delete currentScene_;
	currentScene_ = nullptr;
	//スプライトの解放
	Sprite::Release();
	//モデルの解放
	Model::Release();
	//ポストプロセスの解放
	PostProcess::DeleteInstance();
	//オーディオの解放
	audio_->Finalize();
	//ImGuiの解放
	imguiManager_->ShutDown();
	//TextureManagerの解放
	TextureManager::DeleteInstnace();
	//DirectXCommonの解放
	FCS::DeleteInstance();
	//ゲームウィンドウを閉じる
	winApp_->CloseGameWindow();
}

void GameManager::ChangeScene(IScene* newScene) {
	delete currentScene_;
	currentScene_ = nullptr;
	currentScene_ = newScene;
	currentScene_->Initialize(this);
}

void GameManager::run() {
	while (true) {
		//メッセージ処理
		if (winApp_->ProcessMessage()) {
			break;
		}

		//ImGui受付開始
		imguiManager_->Begin();
		//入力クラスの更新
		input_->Update();
		// ゲームパッドの更新
		gamePad_->Update();
		//グローバル変数の更新
		//GlobalVariables::GetInstance()->Update();
		//ゲームシーンの更新
		currentScene_->Update(this);
		//ポストプロセスの更新
		postProcess_->Update();
		//ImGui受付終了
		imguiManager_->End();

		//描画開始
		dxCommon_->PreDraw();
		//ゲームシーンの描画
		currentScene_->Draw(this);
		//ImGuiの描画
		imguiManager_->Draw();
		//描画終了
		dxCommon_->PostDraw();
	}
}