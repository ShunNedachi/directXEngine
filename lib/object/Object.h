#pragma once
#include<vector>
#include<DirectXTex.h>
#include"MyWindow.h"
#include"MyDirectX12.h"
#include"Camera.h"
#include<string>
#include"Model.h"


#define NORMAL 0
#define TOON 1

class Object
{
private:

	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	Object(int shaderNum);
	~Object();

	// �����������@�S�̂ň��̂�init����
	static void Init(MyDirectX12* my12, MyWindow* window);
	static void CreatePiplineStateOBJ();


	void Draw();

	void Update();

	void CreateModel(const std::string &filename);
	void LoadMaterial(const std::string &directryPath, const std::string &filename);
	bool LoadTexture(const std::string &directoryPath, const std::string &filename);


	// setter
	//void SetModel(Model* model) { this->model = model; }
	void SetShaderNum(int shaderNum) { this->shaderNum = shaderNum; }
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	// getter
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

	DirectX::XMFLOAT4 GetObjColor() { return objColor; }
	void SetObjColor(DirectX::XMFLOAT4 color) { objColor = color; }
private:

	struct ConstBufferDataB0
	{
		//DirectX::XMFLOAT4 clearColor; // �F�iRGBA�j
		DirectX::XMMATRIX mat; // 3d�ϊ��s��
		DirectX::XMFLOAT3 cameraPos;
	};

	// �萔�o�b�t�@�p�f�[�^�\����b1
	struct ConstBufferDataB1
	{
		DirectX::XMFLOAT3 ambient;
		float pad1;
		DirectX::XMFLOAT3 diffuse;
		float pad2;
		DirectX::XMFLOAT3 specular;
		float alpha;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 pos; // xyz���W
		DirectX::XMFLOAT3 normal; // �@���x�N�g��
		DirectX::XMFLOAT2 uv; //	uv���W
	};

	struct Material
	{
		std::string name;
		DirectX::XMFLOAT3 ambient;
		DirectX::XMFLOAT3 diffuse;
		DirectX::XMFLOAT3 specular;
		
		float alpha;
		std::string textureFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};


	#pragma region	directX�p

	// ���L����ϐ�
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature[2]; // ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState[2]; // �p�C�v���C���X�e�[�g
	static DirectX::XMMATRIX matProjection; // �ˉe�s��
	//static const int SRVCount = 512; // �e�N�X�`���̍ő喇��
	static DirectX::XMMATRIX matView;

	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	static MyWindow* window;

	// eye target�̓R���X�g���N�^����
	static Camera* camera;


	// 1�����Ƃɕς���ϐ�
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff; // ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff; // �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB1;

	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_VERTEX_BUFFER_VIEW vbView{}; // ���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW  ibView{}; // �C���f�b�N�X�o�b�t�@�r���[



	DirectX::XMMATRIX matWorld; // ���[���h�s��
	DirectX::XMMATRIX matScale; // �X�P�[�����O�s��
	DirectX::XMMATRIX matRot; // ��]�s��
	DirectX::XMMATRIX matTrans; // ���s�ړ��s��


	DirectX::XMFLOAT3 position = { 0,0,0 }; // ���W
	DirectX::XMFLOAT3 scale = { 1,1,1 };     // �X�P�[��
	DirectX::XMFLOAT3 rotation = { 0,0,0 }; // ��]
	DirectX::XMMATRIX objMatWorld{}; // ���[���h���W
	DirectX::XMFLOAT4 objColor = { 1,1,1,1 }; // �X�v���C�g�̐F
	UINT texNumber = 0; //	�e�N�X�`���ԍ�

	int shaderNum;

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Material material;


#pragma endregion




	//	
};