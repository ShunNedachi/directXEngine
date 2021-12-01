#include "GameScene.h"
#include"ObjectManager.h"
#include"GameFunction.h"

GameScene::~GameScene()
{
	objectManager->Destroy();

}

void GameScene::Initalize()
{
	camera = Camera::GetInstance();
	// 初期画角用
	Camera::SetTheta(20);

	GameFunction::LoadGameSceneTexture();

	objectManager = ObjectManager::GetInstance();

	// 基本objの設置
	objectManager->AddPlayer("sphere");
	objectManager->AddOBJ("ground", { 0,-1,0 });
	objectManager->AddOBJ("skydome");
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddEnemy();
}

void GameScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();

	// カメラの挙動
	if (objectManager->GetIsInitBoss())
	{
		// playerに追従
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		// 敵の登場タイミングで敵にカメラを追従
		camera->Follow(objectManager->GetBossPosition());
		camera->SetPhi(300);
		camera->SetTheta(20);
		camera->SetR(5);
	}

	camera->Update();


	objectManager->Update();

	//// シーン変更
	if (objectManager->GetPlayerDead())NextScene(SceneManager::GetInstance());
	else if (objectManager->GetIsClear())NextScene(SceneManager::GetInstance());

}

void GameScene::Draw()
{
	// ゲームオブジェクトの描画
	objectManager->Draw();
}

void GameScene::NextScene(SceneManager* nowScene)
{
	nowScene->ChangeScene(new EndScene());
}


