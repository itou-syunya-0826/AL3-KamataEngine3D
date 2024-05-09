#include "Matrix.h"
#include <cmath>
using namespace std;

Matrix::Matrix() {}

Matrix4x4 Matrix::MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 result = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate.x, translate.y, translate.z, 1};

	return result;
}

Matrix4x4 Matrix::MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 result = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1};

	return result;
}

Matrix4x4 Matrix::MakeXRotationMatrix(float radian) { 

	Matrix4x4 result = 
	{
		0, 0, 0, 1, 
		0, cosf(radian), sin(radian), 0,
		0, -sinf(radian), cosf(radian), 0,
		0, 0, 0, 1
	};
	
	return result; 
}

Matrix4x4 Matrix::MakeYRotationMatrix(float radian) { 

	Matrix4x4 result = {
		cosf(radian), 0, -sinf(radian), 0,
		0, 1, 0, 0,
		sinf(radian), 0, cosf(radian), 0,
		0, 0, 0, 1
	};

	return result; 
}

Matrix4x4 Matrix::MakeZRotationMatrix(float radian) { 

	Matrix4x4 result = 
	{
	    cosf(radian), sinf(radian), 0, 0,
		-sinf(radian), cosf(radian), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	return result; 
}
