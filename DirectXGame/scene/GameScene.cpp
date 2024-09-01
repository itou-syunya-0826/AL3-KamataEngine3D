#include "GameScene.h"
#include "CameraController.h"
#include "ImGuiManager.h"
#include "Matrix.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete playermodel_;
	delete enemymodel_;
	delete blockmodel_;
	delete deathparticlesmodel_;
	delete goalModel_;
	delete player_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete deathParticles_;
	delete goal_;
	delete skydome_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameracontroller_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	debugCamera_->SetFarZ(5000);

	blockTextureHandle_ = TextureManager::Load("cube/Cube_color.png");
	blockmodel_ = Model::Create();
	playermodel_ = Model::CreateFromOBJ("player", true);
	enemymodel_ = Model::CreateFromOBJ("enemy", true);
	viewProjection_.Initialize();
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_ = new Player();
	player_->Initialize(playermodel_, &viewProjection_, playerPosition);

	enemyTexture_ = TextureManager::Load("kamata.ico");
	Vector3 enemyPosition[10]; 
	enemyPosition[0] = mapChipField_->GetMapChipPositionByIndex(19, 18);
	enemyPosition[1] = mapChipField_->GetMapChipPositionByIndex(22, 13);
	enemyPosition[2] = mapChipField_->GetMapChipPositionByIndex(25, 18);
	enemyPosition[3] = mapChipField_->GetMapChipPositionByIndex(47, 5);
	enemyPosition[4] = mapChipField_->GetMapChipPositionByIndex(50, 18);
	enemyPosition[5] = mapChipField_->GetMapChipPositionByIndex(63, 15);
	enemyPosition[6] = mapChipField_->GetMapChipPositionByIndex(73, 15);
	enemyPosition[7] = mapChipField_->GetMapChipPositionByIndex(83, 13);
	enemyPosition[8] = mapChipField_->GetMapChipPositionByIndex(89, 9);
	enemyPosition[9] = mapChipField_->GetMapChipPositionByIndex(10, 18);
	for (int32_t i = 0; i < 10; i++) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Initialize(enemymodel_, enemyTexture_, &viewProjection_, enemyPosition[i]);
		enemies_.push_back(newEnemy);
	}

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydomeTexture_ = TextureManager::Load("Skydome.png");
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_, skydomeTexture_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	player_->SetMapChipField(mapChipField_);
	for (Enemy* enemy : enemies_) {
		enemy->SetMapChipField(mapChipField_);
	}

	cameracontroller_ = new CameraController();
	cameracontroller_->Initialize(&viewProjection_);
	cameracontroller_->SetTarget(player_);
	cameracontroller_->SetMovableArea({0.f, 180.f, 0.f, 30.f});
	cameracontroller_->Reset();

	deathParticles_ = new Deathparticles;
	deathparticlesmodel_ = Model::CreateFromOBJ("deathparticle", true);
	Vector3 deathParticlePosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	deathParticles_->Initialize(deathparticlesmodel_, &viewProjection_, deathParticlePosition);

	goal_ = new Goal;
	goalModel_ = Model::CreateFromOBJ("Goal", true);
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(94, 7);
	goal_->Initialize(goalModel_, &viewProjection_, goalPosition);

	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	switch (phase_) {
	case Phase::kPlay: // ゲームフェイズの処理

		player_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		skydome_->Update();
		debugCamera_->Update();
		goal_->Update(); //

		if (isdeathparticle_ == true) {
			deathParticles_->Update();
		}

		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}

				worldTransformBlock->matWorld_ = Matrix::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, (Vector3&)worldTransformBlock->translation_);

				worldTransformBlock->TransferMatrix();
			}
		}

#ifdef _DEBUG
		// 排他的論理和
		if (input_->TriggerKey(DIK_BACK)) {
			isDebugCameraActive_ ^= true;
		}

#endif
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();

		} else {
			cameracontroller_->Update();
			viewProjection_.matView = cameracontroller_->GetViewProjection().matView;
			viewProjection_.matProjection = cameracontroller_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
			viewProjection_.UpdateMatrix();
		}
		// 全ての当たり判定を行う
		CheckAllCollisions();

		break;
	case Phase::kDeath: // デス演出フェーズの処理

		skydome_->Update(); // 天球の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update(); // 敵の更新
		}
		deathParticles_->Update(); // デスパーティクルの更新
		debugCamera_->Update();    // カメラの更新↓
#ifdef _DEBUG
		// 排他的論理和
		if (input_->TriggerKey(DIK_BACK)) {
			isDebugCameraActive_ ^= true;
		}

#endif
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();

		} else {
			cameracontroller_->Update();
			viewProjection_.matView = cameracontroller_->GetViewProjection().matView;
			viewProjection_.matProjection = cameracontroller_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		} // ここまで

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}

				worldTransformBlock->matWorld_ = Matrix::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, (Vector3&)worldTransformBlock->translation_);

				worldTransformBlock->TransferMatrix();
			}
		}

		if (deathParticles_->Isfinished()) {
			gameover_ = true;
		}

		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydome_->Draw();
	if (isPlayerDeath) {
		player_->Draw();
	}
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (isdeathparticle_) {
		deathParticles_->Draw();
	}
	goal_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			blockmodel_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 一列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = (Vector3)mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

bool GameScene::IsCollision(const AABB& playerBox, const AABB& enemyBox) {

	bool xCollision = (playerBox.min.x <= enemyBox.max.x && playerBox.max.x >= enemyBox.min.x);
	bool yCollision = (playerBox.min.y <= enemyBox.max.y && playerBox.max.y >= enemyBox.min.y);
	bool zCollision = (playerBox.min.z <= enemyBox.max.z && playerBox.max.z >= enemyBox.min.z);

	return xCollision && yCollision && zCollision;
}

bool GameScene::IsCollision2(const AABB& playerBox, const AABB& goalBox) {
	bool xCollision = (playerBox.min.x <= goalBox.max.x && playerBox.max.x >= goalBox.min.x);
	bool yCollision = (playerBox.min.y <= goalBox.max.y && playerBox.max.y >= goalBox.min.y);
	bool zCollision = (playerBox.min.z <= goalBox.max.z && playerBox.max.z >= goalBox.min.z);

	return xCollision && yCollision && zCollision;
}

void GameScene::CheckAllCollisions() {
#pragma region CharacterCollision

	// 判定対象1と2の座標(aabb1がプレイヤーの当たり判定、aabb2が敵の当たり判定)
	AABB aabb1, aabb2, aabb3;

	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {

			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			// 敵陣の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);

			// 死んだときにパーティクルを出す処理
			if (player_->isDead()) {
				// 死亡時演出フェーズに切り替え
				phase_ = Phase::kDeath;
				// 自キャラの座標を獲得
				const Vector3& deathpartuclesPosition = player_->GetWorldPotision();
				// 自キャラの座標にデスパーティクルを発生、初期化
				deathParticles_->Initialize(deathparticlesmodel_, &viewProjection_, deathpartuclesPosition); // 初期化
			}
			isPlayerDeath = false;// プレイヤーが消える
			isdeathparticle_ = true;// プレイヤーが消えたらパーティクルを出す
		}
	}

	// ゴールの座標
	aabb3 = goal_->GetAABB();
	if (IsCollision(aabb1, aabb3)) {
		// プレイヤーとゴールの衝突時コールバックを呼び出す
		player_->OnCollision(goal_);
		goal_->OnCollision(player_);
		// プレイヤーがゴールに触れてクリアになる処理
		if (player_->isClear()) {
			cleared_ = true;
		}
	
	}

#pragma endregion
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:

		break;

	case Phase::kDeath:

		break;
	}
}
