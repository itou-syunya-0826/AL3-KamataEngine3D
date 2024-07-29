#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include <vector>
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "Deathparticles.h"

// ゲームのフェーズ（型）
enum class Phase {
	kPlay,// ゲームプレイ
	kDeath,// デス演出
};



/// <summary>
/// ゲームシーン
/// </summary>

class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();
	
	bool IsCollision(const AABB& playerBox,const AABB& enemyBox);

	/// <summary>
	/// 全ての当たり判定を行う
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// フェーズの切り替え
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// デスフラグのgetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return finished_; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	uint32_t blockTextureHandle_ = 0;
	ViewProjection viewProjection_;

	Model* blockmodel_ = nullptr;
	Model* playermodel_ = nullptr;
	Model* enemymodel_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* deathparticlesmodel_ = nullptr;
	Player* player_ = nullptr;
	Deathparticles* deathParticles_ = nullptr;

	uint32_t enemyTexture_ = 0u;
	Skydome* skydome_ = nullptr;
	uint32_t skydomeTexture_ = 0u;
	DebugCamera* debugCamera_ = nullptr;
	MapChipField* mapChipField_=nullptr;
	CameraController* cameracontroller_ = nullptr;

	// ゲームの現在フェーズ（変数）
	Phase phase_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	std::list<Enemy*> enemies_;

	bool isPlayerDeath = true;
	bool isDebugCameraActive_ = false;
	bool isdeathparticle_ = false;
	// 終了フラグ
	bool finished_ = false;
	
};
