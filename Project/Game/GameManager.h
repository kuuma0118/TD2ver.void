#include "IScene.h"
#include "Engine/Base/D3DResourceLeakChecker.h"
#include "Engine/Base/WinApp.h"
#include "Engine/Base/FireControlSystem.h"
#include "Engine/Base/TextureManager.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/2D/Sprite.h"
#include "Engine/2D/ImGuiManager.h"
#include "Engine/Components/Input.h"
#include "Engine/Components/Audio.h"
#include "Engine/Components/PostProcess.h"
#include "3D/Model/Model.h"
#include "3D/Model/ParticleModel.h"

#include "Project/Components/GamePad.h"
#include <memory>

class GameManager {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

	/// <summary>
	/// シーン切り替え
	/// </summary>
	/// <param name="newScene"></param>
	void ChangeScene(IScene* newScene);

	/// <summary>
	/// ゲームループ
	/// </summary>
	void run();

private:
	//リークチェッカー
	//D3DResourceLeakChecker leakCheck;
	//ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;
	//DirectX基盤クラス
	FCS* dxCommon_ = nullptr;
	//テクスチャ管理クラス
	TextureManager* textureManager_ = nullptr;
	//ImGuiManager
	ImGuiManager* imguiManager_ = nullptr;
	//入力クラス
	Input* input_ = nullptr;
	// ゲームパッド
	GamePad* gamePad_ = nullptr;
	//オーディオクラス
	Audio* audio_ = nullptr;
	//ポストプロセス
	PostProcess* postProcess_ = nullptr;
	//現在のシーン
	IScene* currentScene_ = nullptr;

};