#pragma once
#include <ViewProjection.h>

class CameraController {
private:

	ViewProjection* viewProjection_ = nullptr;


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

};
