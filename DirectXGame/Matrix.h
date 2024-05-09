#pragma once
#include <Matrix4x4.h>
#include <Vector3.h>


class Matrix 
{
private:

public:

	Matrix();
	/// <summary>
	/// 1.平行移動行列
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	/// <summary>
	/// 2.拡大縮小行列
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	/// <summary>
	/// 3.X軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeXRotationMatrix(float radian);
	/// <summary>
	/// 4.Y軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeYRotationMatrix(float radian);
	/// <summary>
	/// 5.Z軸回転行列
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	static Matrix4x4 MakeZRotationMatrix(float radian);

};
