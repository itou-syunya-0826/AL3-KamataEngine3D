#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = Matrix::MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}