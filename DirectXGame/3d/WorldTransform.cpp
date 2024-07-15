#include "WorldTransform.h"
#include "Matrix.h"

void WorldTransform::UpdateMatrix() { 
	// スケール、回転、平行移動を用いてアフィン行列を作成
	matWorld_ = Matrix::MakeAffineMatrix(scale_, rotation_, (Vector3&)translation_);

	// 行列を転送
	TransferMatrix();
}