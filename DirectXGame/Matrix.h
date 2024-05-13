#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>


class Matrix
{
private:

public:

	Matrix();

	/// <summary>
    /// 1.行列の加法
    /// </summary>
    /// <param name="m1"></param>
    /// <param name="m2"></param>
    /// <returns></returns>
	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
	/// <summary>
	/// 2.行列の減法
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
	/// <summary>
	/// 3.行列の積
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	/// <summary>
	/// 4.逆行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 Inverse(const Matrix4x4& matrix);
	/// <summary>
	/// 5.転置行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 Transpose(const Matrix4x4& matrix);
	/// <summary>
	/// 6.単位行列の作成
	/// </summary>
	/// <returns></returns>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// 1.平行移動行列
	/// </summary>
	/// <param name="translate">トランスレート</param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	/// <summary>
	/// 2.拡大縮小行列
	/// </summary>
	/// <param name="scale">大きさ</param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	/// <summary>
	/// 3.座標変換
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="matrix">行列</param>
	/// <returns></returns>
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	/// <summary>
	/// 1.X軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateXMatrix(float radian);
	/// <summary>
	/// 2.Y軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateYMatrix(float radian);
	/// <summary>
	/// 3.Z軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeRotateZMatrix(float radian);

	static Matrix4x4 MakeRotateXYZMatrix(Vector3 radian);

	/// <summary>
	/// 1.3次元アフィン変換行列
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

};

