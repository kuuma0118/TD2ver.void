#pragma once
#include <Windows.h>
#include <cstdint>
#include "Engine/Externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/// <summary>
/// ウィンドウズアプリケーション
/// </summary>
class WinApp {
public:
	//ウィンドウサイズ
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="wparam"></param>
	/// <param name="lparam"></param>
	/// <returns></returns>
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ゲームウィンドウの作成
	/// </summary>
	/// <param name="title"></param>
	/// <param name="clientWidth"></param>
	/// <param name="clientHeight"></param>
	void CreateGameWindow(const wchar_t* title, int32_t clientWidth, int32_t clientHeight);

	/// <summary>
	/// ゲームウィンドウを閉じる
	/// </summary>
	void CloseGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const { return hwnd_; };

	/// <summary>
	/// インスタンスハンドルの取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetHInstance() { return wc_.hInstance; };

private:
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private:
	//ウィンドウクラス
	WNDCLASS wc_{};
	//ウィンドウサイズ
	RECT wrc_{};
	//ウィンドウハンドル
	HWND hwnd_{};

};