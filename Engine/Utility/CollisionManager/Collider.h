#pragma once
#include <stdint.h>
#include "CollisionConfig.h"
#include "Engine/Utility/MathFunction.h"

class Collider {
public:
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision(Collider* collider) {};

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; };

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float radius) { radius_ = radius; };

	/// <summary>
	/// AABBのサイズを取得
	/// </summary>
	/// <returns></returns>
	AABB GetAABB() { return aabb_; };

	/// <summary>
	/// AABBのサイズを設定
	/// </summary>
	/// <param name="aabb"></param>
	void SetAABB(AABB& aabb) { aabb_ = aabb; };

	/// <summary>
	/// 衝突属性(自分)を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; };

	/// <summary>
	/// 衝突属性(自分)を設定
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; };

	/// <summary>
	/// 衝突属性(相手)を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionMask() { return collisionMask_; };

	/// <summary>
	/// 衝突属性(相手)を設定
	/// </summary>
	/// <param name="collisionMask"></param>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; };

	/// <summary>
	/// 形状を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetCollisionPrimitive() { return collisionPrimitive_; };

	/// <summary>
	/// 形状を設定
	/// </summary>
	/// <param name="collisionPrimitive"></param>
	void SetCollisionPrimitive(uint32_t collisionPrimitive) { collisionPrimitive_ = collisionPrimitive; };

	/// <summary>
	/// ダメージを取得
	/// </summary>
	/// <returns></returns>
	float GetDamage() { return damage_; };

	/// <summary>
	/// ダメージを設定
	/// </summary>
	/// <param name="damage"></param>
	void SetDamage(float damage) { damage_ = damage; };


	/// <summary>
	/// AABBの下面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsBottomHitAABB() { return isBottomHitAABB_; }

	/// <summary>
	/// AABBの下面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsBottomHitAABB(bool isActive) { isBottomHitAABB_ = isActive; }

	/// <summary>
	/// AABBの上面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsTopHitAABB() { return isTopHitAABB_; }

	/// <summary>
	/// AABBの上面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsTopHitAABB(bool isActive) { isTopHitAABB_ = isActive; }

	/// <summary>
	/// AABBの右面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsRightHitAABB() { return isRightHitAABB_; }

	/// <summary>
	/// AABBの右面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsRightHitAABB(bool isActive) { isRightHitAABB_ = isActive; }

	/// <summary>
	/// AABBの左面部分に当たったかを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsLeftHitAABB() { return isLeftHitAABB_; }

	/// <summary>
	/// AABBの左面部分に当たったかを設定
	/// </summary>
	/// <param name="isActive"></param>
	void SetIsLeftHitAABB(bool isActive) { isLeftHitAABB_ = isActive; }

	bool GetIsDelete() { return isDelete_; }

	void SetIsDelete(bool isActive) { isDelete_ = isActive; }

private:
	//衝突半径
	float radius_ = 1.0f;
	//AABB
	AABB aabb_{ {-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f} };
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;
	//形状(デフォルトは球に設定)
	uint32_t collisionPrimitive_ = kCollisionPrimitiveSphere;
	//ダメージ
	float damage_ = 1.0f;

	// 下方向に当たってる
	bool isBottomHitAABB_ = false;
	// 上方向に当たってる
	bool isTopHitAABB_ = false;
	// 右方向に当たってる
	bool isRightHitAABB_ = false;
	// 左方向に当たってる
	bool isLeftHitAABB_ = false;

	// 解放処理をしてもよいか
	bool isDelete_ = false;
};