#pragma once
#include "Engine/Components/Input.h"
#include "Engine/3D/Model/Model.h"

#include "Engine/Base/TextureManager.h"
class Block{

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Block();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Block();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private: // メンバ変数

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	//3Dモデル
	std::unique_ptr<Model> model_{};


	//キーボード入力
	Input* input_ = nullptr;

	//テクスチャハンドル
	uint32_t texHandle_ = 0;
	uint32_t texHandleBullet_ = 0;
};

