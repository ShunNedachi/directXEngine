#include<DirectXMath.h>
#include<vector>
#include<DirectXTex.h>
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Input.h"
#include<wrl.h>
#include"Sprite2D.h"
#include"Object.h"
#include"Camera.h"



using namespace Microsoft::WRL;


bool MessageError(MSG msg);

// windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	using namespace DirectX;

	#pragma region ����������
	// �E�B���h�E�p
	MyWindow myw;
	myw.Initialize("Engine");

	Input input;
	input.Initialize(myw.GetConfig().hInstance,myw.GetHWND());

	// DirectX ���������� ��������
	MyDirectX12 my12;
	HRESULT result;

	my12.Debug();
	my12.Initialize(&myw);

	Sprite2D::Init(&my12, &myw);

	Camera* camera = Camera::GetInstance();


	// DirectX ���������� �����܂�

#pragma endregion



	// �`�揉����


	// �X�v���C�g
	Sprite2D titleSprite(0.5f,0.5f);

	Sprite2D::LoadTex(0, L"Resources/texture/title.png");

	titleSprite.CreateSprite(0);
    titleSprite.SetPosition({ 650,600,0 });


	// �@3D�I�u�W�F�N�g
	Object* obj = new Object();
	obj->Init(&my12, &myw,camera);
	obj->CreateModel("bless");

	Object* obj2 = new Object();
	obj2->CreateModel("bless");
	

	MSG msg{}; // ���b�Z�[�W
	while (true)
	{
		if (MessageError(msg)) { break; };

		// �X�V
		input.Update();

		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 target = camera->GetTarget();

		if (input.PushKey(DIK_W))
		{
			eye.z--;
		}
		else if (input.PushKey(DIK_S))
		{
			eye.z++;
		}

	
		camera->SetEye(eye);
	/*	obj->SetTarget(obj->GetPosition());
		obj->SetEye(eye);*/

		obj->Update({ 0,0,0 });
		obj2->Update({ 0,1,0 });
		#pragma region �`�揈��

		// �`�揀���p
		my12.PreDraw();

		//obj->Draw(NORMAL);
		obj->Draw(TOON);

		// �`��R�}���h�����p
		my12.PostDraw();

		#pragma endregion

	}

	delete obj;
	delete obj2;
	camera->Destroy();

	return 0;
}

bool MessageError(MSG msg)
{
	// ���b�Z�[�W������H
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}
	// �I�����b�Z�[�W�������烋�[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	// 0�̂Ƃ����퓮��
	return false;
}