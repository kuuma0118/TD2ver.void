#pragma once
#include "Engine/Base/WinApp.h"
#include "Engine/Base/FireControlSystem.h"
#include "Engine/Externals/imgui/imgui.h"
#include "Engine/Externals/imgui/imgui_impl_dx12.h"

/// <summary>
/// ImGuiを管理するクラス
/// </summary>
class ImGuiManager{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ImGuiManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの解放処理
	/// </summary>
	void ShutDown();

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator=(const ImGuiManager&) = delete;

private:
	//ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;
	//DirectXCommon
	FCS* dxCommon_ = nullptr;
	//SRV用ヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;

};

