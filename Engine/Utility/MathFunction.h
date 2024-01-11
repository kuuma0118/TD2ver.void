#pragma once
#include "MathStruct.h"
#include <cmath>
#include <cassert>

/// <summary>
/// ベクトルの足し算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルの引き算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// ベクトルの掛け算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Multiply(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 掛け算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Multiply(const float& v1, const Vector3& v2);

/// <summary>
/// 正規化
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v);

/// <summary>
/// 長さを計算
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
float Length(const Vector3& v);

/// <summary>
/// 行列の引き算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 行列の掛け算
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 逆行列の計算
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
/// 単位行列の作成
/// </summary>
/// <returns></returns>
Matrix4x4 MakeIdentity4x4();

/// <summary>
/// 平行移動行列の作成
/// </summary>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// スケール行列の作成
/// </summary>
/// <param name="scale"></param>
/// <returns></returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// X軸回転行列の作成
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸回転行列の作成
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸回転行列の作成
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// アフィン行列の作成
/// </summary>
/// <param name="scale"></param>
/// <param name="rotate"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// 透視投影行列の作成
/// </summary>
/// <param name="fovY"></param>
/// <param name="aspectRatio"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// 平行投影行列の作成
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
float Lerp(const float& v1, const float& v2, float t);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
