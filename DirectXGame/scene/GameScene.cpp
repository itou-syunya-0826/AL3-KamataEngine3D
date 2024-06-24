#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Matrix.h"
#include "ImGuiManager.h"
#include "CameraController.h"

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete blockmodel_;
	delete player_;
	delete skydome_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete cameracontroller_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine){
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

	blockTextureHandle_ = TextureManager::Load("cube/cube.jpg");
	blockmodel_ = Model::Create();
	viewProjection_.Initialize();
	//座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1,18 );
	player_ = new Player();
	playerTexture_ = TextureManager::Load("kamata.ico");
	player_->Initialize(blockmodel_, playerTexture_, &viewProjection_, playerPosition);
	

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydomeTexture_ = TextureManager::Load("uvChecker.png");
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjection_,skydomeTexture_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();
	player_->SetMapChipField(mapChipField_);

	cameracontroller_ = new CameraController();
	cameracontroller_->Initialize(&viewProjection_);
	cameracontroller_->SetTarget(player_); 
	cameracontroller_->SetMovableArea({0.f, 180.f, 0.f, 30.f});
	cameracontroller_->Reset();
	
}

void GameScene::Update() { 
	player_->Update(); 
	skydome_->Update(); 
	debugCamera_->Update();
	

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
	//排他的論理和
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
	player_->Draw();
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
	//要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		//一列の要素数を設定（横方向のブロック数）
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
