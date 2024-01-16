#pragma once
#include "Engine//Utility/CollisionManager/Collider.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/3D/Matrix/WorldTransform.h"
#include <memory>

class Block
{
public:
	/// 
	/// Default Method
	/// 

	Block() = default;
	~Block();

	// 初期化
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Draw(const ViewProjection& viewProjection);

private:
	// 移動処理用のワールド座標
	WorldTransform worldTransform_;

	// モデル
	std::unique_ptr<Model> model_;
};