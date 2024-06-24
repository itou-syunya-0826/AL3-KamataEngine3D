#pragma once
#include <Vector3.h>


Vector3 operator+(const Vector3& mValue, const float& rhs);


Vector3 operator+(const Vector3& mValue, const Vector3& rhs);

const Vector3& operator+=(Vector3& mValue, const Vector3& rhs);

//Vector3 operator+(const MyVector3& rhs1, const MyVector3& rhs2) {
//	Vector3 result;
//	result.x = rhs1.mValue.x + rhs2.mValue.x;
//	result.y = rhs1.mValue.y + rhs2.mValue.y;
//	result.z = rhs1.mValue.z + rhs2.mValue.z;
//	return result;
//}
//Vector3 operator+(const Vector3& rhs1, const Vector3& rhs2) {
//	Vector3 result;
//	result.x = rhs1.x + rhs2.x;
//	result.y = rhs1.y + rhs2.y;
//	result.z = rhs1.z + rhs2.z;
//	return result;
//}