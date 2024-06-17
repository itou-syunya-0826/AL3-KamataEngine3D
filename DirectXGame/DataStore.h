#pragma once
#include <Vector3.h>

class MyVector3 {
public:
	 MyVector3& operator+=(const MyVector3& rhs) {
		mValue.x += rhs.mValue.x;
		mValue.y += rhs.mValue.y;
		mValue.z += rhs.mValue.z;
		return *this;
	 }
	 Vector3 operator+(const Vector3& rhs) {
		 Vector3 result;
		 result.x = mValue.x + rhs.x;
		 result.y = mValue.y + rhs.y;
		 result.z = mValue.z + rhs.z;
		 return result;
	 }

public:
	Vector3 mValue;
};

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