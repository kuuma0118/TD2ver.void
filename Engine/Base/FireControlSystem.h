#pragma once
#include "Engine/Base/WinApp.h"
#include "Engine/Utility/Log.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <chrono>
#include <cassert>
#include <wrl.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"winmm.lib")

/// <summary>
/// DirectX基本
/// </summary>
class FCS final {
public:
	//インクリメントサイズ
	static uint32_t descriptorSizeRTV;
	static uint32_t descriptorSizeDSV;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static FCS* GetInstance();

	/// <summary>
	/// シングルトンインスタンスの削除
	/// </summary>
	static void DeleteInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// バックバッファをセット
	/// </summary>
	void SetBackBuffer();

	/// <summary>
	/// レンダーターゲットをクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// デプスバッファのクリア
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// ディスクリプタヒープの作成
	/// </summary>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// リソースの作成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() const { return device_.Get(); };

	/// <summary>
	/// コマンドリストを取得
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); };

private:
	FCS() = default;
	~FCS() = default;
	FCS(const FCS&) = delete;
	FCS& operator=(const FCS&) = delete;

	/// <summary>
	/// DXGIデバイスの作成
	/// </summary>
	void CreateDXGIDevice();

	/// <summary>
	/// コマンド関連の作成
	/// </summary>
	void CreateCommand();

	/// <summary>
	/// スワップチェーンの作成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// レンダーターゲットビューの作成
	/// </summary>
	void CreateRenderTargetView();

	/// <summary>
	/// DepthStencilViewの作成
	/// </summary>
	void CreateDepthStencilView();

	/// <summary>
	/// フェンスの作成
	/// </summary>
	void CreateFence();

	/// <summary>
	/// DepthStencilTextureの作成
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(int32_t width, int32_t height);

	/// <summary>
	/// FPS固定初期化
	/// </summary>
	void InitializeFixFPS();

	/// <summary>
	/// FPS固定更新
	/// </summary>
	void UpdateFixFPS();

private:
	//インスタンス
	static FCS* instance;
	//ウィンドウズアプリケーション
	WinApp* winApp_ = nullptr;
	//DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;
	//アダプタ
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;
	//デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	//コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	//コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	//スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	//ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_{};
	//レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_{};

};