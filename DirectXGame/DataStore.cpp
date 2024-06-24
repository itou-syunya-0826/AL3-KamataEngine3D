#include "DataStore.h"

Vector3 operator+(const Vector3& mValue, const float& rhs) {
		Vector3 result;
		result.x = mValue.x + rhs;
		result.y = mValue.y + rhs;
		result.z = mValue.z + rhs;
		return result;
}

Vector3 operator+(const Vector3& mValue, const Vector3& rhs) {
	Vector3 result;
	result.x = mValue.x + rhs.x;
	result.y = mValue.y + rhs.y;
	result.z = mValue.z + rhs.z;
	return result;
}

const Vector3& operator+=(Vector3& mValue, const Vector3& rhs) {
	mValue.x += rhs.x;
	mValue.y += rhs.y;
	mValue.z += rhs.z;
	return mValue;
}
