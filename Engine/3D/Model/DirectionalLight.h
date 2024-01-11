#pragma once
#include "Engine/Utility/MathStruct.h"
#include <cstdint>
#include <d3d12.h>
#include <wrl.h>

/// <summary>
/// ライト
/// </summary>
class DirectionalLight {
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	/// <summary>
	/// ライティングの種類
	/// </summary>
	enum class LightingMethod {
		LambertianReflectance,
		HalfLambert
	};

	/// <summary>
	/// 定数バッファ用の構造体
	/// </summary>
	struct ConstBufferDataDirectionalLight {
		//フラグ
		int32_t enableLighting;
		//ライティングの種類
		LightingMethod lightingMethod;
		//パディング
		float padding[2];
		//ライトの色
		Vector4 color;
		//ライトの向き
		Vector3 direction;
		//輝度
		float intensity;
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// グラフィックスコマンドのセット
	/// </summary>
	void SetGraphicsCommand(UINT rootParameterIndex);

	/// <summary>
	/// ライティングのフラグを取得
	/// </summary>
	/// <returns></returns>
	const int32_t& GetEnableLighting() const { return enableLighting_; };

	/// <summary>
	/// ライティングのフラグを設定
	/// </summary>
	/// <param name="enableLighting"></param>
	void SetEnableLighting(const int32_t& enableLighting) { enableLighting_ = enableLighting; };

	/// <summary>
	/// ライティングの種類を取得
	/// </summary>
	/// <returns></returns>
	const int32_t& GetLightingMethod() const { return int32_t(lightingMethod_); };

	/// <summary>
	/// ライティングの種類を設定
	/// </summary>
	/// <param name="lightingMethod"></param>
	void SetLightingMethod(const int32_t& lightingMethod) { lightingMethod_ = LightingMethod(lightingMethod); };

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
	/// 向きを取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetDirection() const { return direction_; };

	/// <summary>
	/// 向きを設定
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(const Vector3& direction) { direction_ = direction; };

	/// <summary>
	/// 輝度を取得
	/// </summary>
	/// <returns></returns>
	const float& GetIntensity() const { return intensity_; };

	/// <summary>
	/// 輝度を設定
	/// </summary>
	/// <param name="intensity"></param>
	void SetIntensity(const float& intensity) { intensity_ = intensity; };

private:
	//ライティング用のリソース
	ComPtr<ID3D12Resource> lightingResource_ = nullptr;
	//ライティングのフラグ
	int32_t enableLighting_ = true;
	//ライティング方式
	LightingMethod lightingMethod_ = LightingMethod::HalfLambert;
	//ライトの色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
	//ライトの向き
	Vector3 direction_ = { 0.0f,-1.0f,0.0f };
	//輝度
	float intensity_ = 1.0f;

};