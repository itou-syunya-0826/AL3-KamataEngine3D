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

public:
	Vector3 mValue;
};
