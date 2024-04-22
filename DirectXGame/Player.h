#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Player {
public:

	Player();
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model_">モデルのポインタ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// ワールド変換データ
	/// </summary>
	WorldTransform worldTransform_;
	/// <summary>
	/// モデル
	/// </summary>
	Model* model_ = nullptr;
	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	uint32_t textureHandle_ = 0;

};
