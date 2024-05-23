#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = Matrix::MakeAffineMatrix(scale_, rotation_, (Vector3&)translation_);

	TransferMatrix();
}