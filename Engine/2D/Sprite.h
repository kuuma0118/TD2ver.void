#pragma once
#include "Engine/Base/FireControlSystem.h"
#include "Engine/Base/TextureManager.h"
#include "Engine/Utility/MathFunction.h"
#include <array>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

/// <summary>
/// スプライト
/// </summary>
class Sprite{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//ブレンドモード
	enum BlendMode {
		//ブレンドなし
		kBlendModeNone,
		//通常aブレンド
		kBlendModeNormal,
		//加算
		kBlendModeAdd,
		//減算
		kBlendModeSubtract,
		//乗算
		kBlendModeMultiply,
		//スクリーン
		kBlendModeScreen,
		//利用してはいけない
		kCountOfBlendMode,
	};

	/// <summary>
	/// 頂点データ
	/// </summary>
	struct VertexData {
		Vector4 position{};
		Vector2 texcoord{};
	};

	/// <summary>
	/// マテリアルデータ
	/// </summary>
	struct MaterialData {
		Vector4 color{};
		Matrix4x4 uvTransform{};
	};

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// スプライトの静的解放
	/// </summary>
	static void Release();

	/// <summary>
	/// スプライトの作成
	/// </summary>
	/// <param name="textureHandle"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	static Sprite* Create(uint32_t textureHandle, Vector2 position);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="blendMode"></param>
	static void PreDraw(BlendMode blendMode);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector2& GetPosition() const { return position_; };

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector2& position) { position_ = position; };

	/// <summary>
	/// 角度を取得
	/// </summary>
	/// <returns></returns>
	const float& GetRotation() const { return rotation_; };

	/// <summary>
	/// 角度を設定
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const float& rotation) { rotation_ = rotation; };

	/// <summary>
	/// サイズを取得
	/// </summary>
	/// <returns></returns>
	const Vector2& GetSize() const { return size_; };

	/// <summary>
	/// サイズを設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(const Vector2& size) { size_ = size; };

	/// <summary>
	/// 色を取得
	/// </summary>
	/// <returns></returns>
	const Vector4& GetColor() const { return color_; };

	/// <summary>
	/// 色を設定
	/// </summary>
	/// <param name="color"></param>
	void SetColor(const Vector4& color) { color_ = color; };

	/// <summary>
	/// uv座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector2 GetUVTranslation() const { return uvTranslation_; };

	/// <summary>
	/// uv座標を設定
	/// </summary>
	/// <param name="uvTranslation"></param>
	void SetUVTranslation(const Vector2& uvTranslation) { uvTranslation_ = uvTranslation; };

	/// <summary>
	/// uv角度を取得
	/// </summary>
	/// <returns></returns>
	const float GetUVRotation() const { return uvRotation_; };

	/// <summary>
	/// uv角度を設定
	/// </summary>
	/// <param name="uvRotation"></param>
	void SetUVRotation(const float& uvRotation) { uvRotation_ = uvRotation; };

	/// <summary>
	/// uvスケールを取得
	/// </summary>
	/// <returns></returns>
	const Vector2 GetUVScale() const { return uvScale_; };

	/// <summary>
	/// uvスケールを設定
	/// </summary>
	/// <param name="uvScale"></param>
	void SetUVScale(const Vector2& uvScale) { uvScale_ = uvScale; };

private:
	/// <summary>
	/// DXCの初期化
	/// </summary>
	static void InitializeDXC();

	/// <summary>
	/// シェーダーをコンパイルする
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <returns></returns>
	static ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// PSOの作成
	/// </summary>
	static void CreatePipelineStateObject();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t textureHandle, Vector2 position);

	/// <summary>
	/// 頂点バッファの作成
	/// </summary>
	void CreateVertexBuffer();

	/// <summary>
	/// マテリアル用のリソースの作成
	/// </summary>
	void CreateMaterialResource();

	/// <summary>
	/// マテリアルの更新
	/// </summary>
	void UpdateMaterial();

	/// <summary>
	/// WVP用のリソースの作成
	/// </summary>
	void CreateWVPResource();

	/// <summary>
	/// 行列の計算・転送
	/// </summary>
	void UpdateMatrix();

private:
	//デバイス
	static ID3D12Device* sDevice_;
	//コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	//DXC
	static ComPtr<IDxcUtils> sDxcUtils_;
	static ComPtr<IDxcCompiler3> sDxcCompiler_;
	static ComPtr<IDxcIncludeHandler> sIncludeHandler_;
	//RootSignature
	static ComPtr<ID3D12RootSignature> sRootSignature_;
	//PipelineStateObject
	static std::array<ComPtr<ID3D12PipelineState>, kCountOfBlendMode> sGraphicsPipelineState_;
	//射影行列
	static Matrix4x4 sMatProjection_;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//座標
	Vector2 position_ = { 0.0f,0.0f };
	//角度
	float rotation_ = 0.0f;
	//サイズ
	Vector2 size_ = { 1.0f,1.0f };
	//テクスチャ始点
	Vector2 texBase_ = { 0.0f,0.0f };
	//テクスチャサイズ
	Vector2 texSize_ = { 1.0f,1.0f };
	//テクスチャハンドル
	uint32_t textureHandle_{};
	//テクスチャの情報
	D3D12_RESOURCE_DESC resourceDesc_{};
	//マテリアル用のリソース
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	//WVP用のリソース
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	//色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//uv座標
	Vector2 uvTranslation_ = { 0.0f,0.0f };
	//uv角度
	float uvRotation_ = 0.0f;
	//uvスケール
	Vector2 uvScale_ = { 1.0f,1.0f };

};

