#pragma once
#include "3D/Matrix/ViewProjection.h"
#include "3D/Matrix/WorldTransform.h"
//
//class Camera {
//public:
//	/// 
//	/// Default Method
//	/// 
//
//	// 初期化
//	void Initialize();
//
//	// 更新処理
//	void Update();
//
//	/// 
//	/// User Method
//	/// 
//
//	/// Getter
//	/// ビュープロジェクションを取得
//	const ViewProjection& GetViewProjection() { return viewProjection_; }
//
//	/// Setter
//	// 追従する対象を設定
//	void SetTarget(const WorldTransform* target) { target_ = target; }
//
//private:
//	ViewProjection viewProjection_;
//	const WorldTransform* target_ = nullptr;
//};