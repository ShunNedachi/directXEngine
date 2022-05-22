#pragma once
#include "Scene.h"
#include"TitleScene.h"
#include"EndScene.h"
#include"ClearScene.h"
#include<ObjectManager.h>
#include"Camera.h"
#include"Player.h"
#include"Particle2D.h"

class GameScene :
    public Scene
{
	~GameScene();

	// �V�[���̓���p
	void Initalize() override;
	void Update() override;
	void Draw() override;

	// �V�[���̐؂�ւ�
	void NextScene(SceneManager* nowScene) override;

public:

	// �֐�

private:

	// �J����
	Camera* camera;

};
