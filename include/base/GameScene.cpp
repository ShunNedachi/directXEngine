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
	objectManager->AddOBJ("ground", { 0,-1,0 }, { 10,10,10 });
	objectManager->AddOBJ("skydome", { 0,0,0 }, { 5,5,5 });
	//camera->SetEye({ 0, 20, -100 });

	objectManager->AddBoss();
}

void GameScene::Update()
{
	// input関係
	Input* input = Input::GetInstance();
	Xinput* xinput = Xinput::GetInstance();
	
	
	objectManager->Update();
	camera->Update();

	// カメラの挙動
	if (objectManager->GetIsInitBoss())
	{
		// playerに追従
		camera->Follow(objectManager->GetPlayerPos());
	}
	else
	{
		const float R = 10;
		const float Phi = 300;
		const float Theta = 20;

		// 敵の登場タイミングで敵にカメラを追従
		camera->SetState(R, Theta, Phi);
		camera->Follow(objectManager->GetBossPosition());
	}
	

	//// シーン変更
	if (objectManager->GetPlayerDead())ChangeScene(SceneManager::GetInstance(),endSceneNum);
	else if (objectManager->GetIsClear())ChangeScene(SceneManager::GetInstance(),clearSceneNum);

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


