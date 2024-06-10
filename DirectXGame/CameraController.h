#pragma once
#include <ViewProjection.h>

//Player.hをインクルードせず前方宣言する。なぜなら多重実装になるから
class Player;

class CameraController {
private:

	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	/*ViewProjection viewProjection_;*/
	ViewProjection* viewProjection_ = nullptr;
	Player* target_ = nullptr;
	//追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -30.f};
	//カメラ移動範囲
	Rect movableArea_ = {0,100,0,100};
	//カメラの目標座標
	Vector3 targetCoordinates_ = {};
	//座標補間割合
	static inline const float kInterpolationRate = 0.2f;
	//速度掛け率
	static inline const float kVelocityBias = 20.f;

	static inline const Rect kMargin = {5, 10, 5, 10};


public:

	CameraController();
	~CameraController();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ViewProjection* viewProjection);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(Player* target) { target_ = target; }

	void SetMovableArea(Rect area) { movableArea_ = area; } 

	void Reset();

	const ViewProjection& GetViewProjection() { return *viewProjection_; }

};
