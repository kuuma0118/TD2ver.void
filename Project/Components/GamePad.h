#pragma once
#include "Engine/Base/WinApp.h"
#include <memory>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")

class GamePad
{
public:
	///
	/// Default Method
	/// 
	
	// シングルトン
	static GamePad* GetInstance();

	// 初期化
	//void Initialize();
	
	// 更新処理
	void Update();

	///
	/// User Method
	/// 

	/// <summary>
	/// コントローラーの状態を取得
	/// </summary>
	/// <returns></returns>
	bool GetJoystickState(XINPUT_STATE& state);

	// ボタンを押した瞬間
	bool TriggerButton(int GAMEPAD_NUM);
	// ボタンを離した瞬間
	bool ReleaseButton(int GAMEPAD_NUM);
	// ボタンを押している間
	bool PressButton(int GAMEPAD_NUM);

private:
	GamePad() = default;
	~GamePad() = default;

private:
	//コントローラーの情報
	XINPUT_STATE state_{};
	// 1フレーム前コントローラの情報
	XINPUT_STATE preState_{};
};

