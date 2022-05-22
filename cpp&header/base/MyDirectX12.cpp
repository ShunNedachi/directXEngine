#include "MyDirectX12.h"
#include"Setting.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
#include<vector>
#include<d3dcompiler.h>
#include<d3dx12.h>
#include<imgui_impl_win32.h>
#include<imgui_impl_dx12.h>

#include<memory>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

MyDirectX12::MyDirectX12()
{
}
MyDirectX12::~MyDirectX12()
{
	#ifdef _DEBUG

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif // _DEBUG

}


void MyDirectX12::Initialize(MyWindow* window)
{
	this->window = window;

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter> tmpAdapter;
	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter.Get()); // ���I�z��ɒǉ�����
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc); // �A�_�v�^�[�̏����擾
		std::wstring strDesc = adesc.Description; // �A�_�v�^�[��
		// Microsoft Basic Render Driver�����
		if (strDesc.find(L"Microsoft") == std::wstring::npos
			&& strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // �@�̗p
			break;
		}
	}



	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	// �R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	// �R�}���h���X�g�𐶐�
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	ComPtr<IDXGISwapChain1> swapchain1;
	swapchainDesc.Width = SETTING_VALUE::WINDOW_WIDTH;
	swapchainDesc.Height = SETTING_VALUE::WINDOW_HEIGHT;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2; // �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		window->GetHWND(),
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain1);

	//�@��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�
	swapchain1.As(&swapchain);


	// �e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2; // �\���̂Q��
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	// �\���̂Q���ɂ���


	for (int i = 0; i < 2; i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		//D3D12_CPU_DESCRIPTOR_HANDLE handle =
		//	rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//// �\�������ŃA�h���X�������
		//handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);

		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));

		// �����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}

	// �t�F���X�̐���
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	// �[�x�r���[�p
	CreatDepthDesc();

	#ifdef _DEBUG

		// imgui 
		InitImgui();

#endif // _DEBUG

}

void MyDirectX12::Debug()
{
#if defined(_DEBUG)
	// �f�o�b�O���C���[���I����

	//ID3D12Debug* debugController;
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController.Get()->EnableDebugLayer();
	}

#endif // DEBUG
}

Microsoft::WRL::ComPtr<ID3D12Device> MyDirectX12::Device()
{
	return dev;
}
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> MyDirectX12::CommandList()
{
	return cmdList;
}

void MyDirectX12::BeginBarrier()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾
	bbIndex = swapchain->GetCurrentBackBufferIndex();

	//// ���\�[�X�o���A�̕ύX	

	// �\����Ԃ���`���ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
}
void MyDirectX12::EndBarrier()
{
	// 4.���\�[�X�o���A��߂�

	// �`���Ԃ���\����ԂɕύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void MyDirectX12::ClearScreen()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);


	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �S��ʃN���A�@�@�@�@ R G B A
	float clearColor[] = { clearBackColor.x,clearBackColor.y,clearBackColor.z,clearBackColor.w }; // �N���A�J���[
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}


void MyDirectX12::ExecuteCmd()
{
	// ���߂̃N���[�Y
	cmdList->Close();
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList.Get() }; // �R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	// �R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator->Reset(); // �L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr); // �ĂуR�}���h���X�g�����߂鏀��

	// �o�b�t�@���t���b�v(�\���̓���ւ�)
	swapchain->Present(1, 0);
}


void MyDirectX12::SetViewport()
{
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, SETTING_VALUE::WINDOW_WIDTH, SETTING_VALUE::WINDOW_HEIGHT));
}

void MyDirectX12::SetScissorrect()
{
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, SETTING_VALUE::WINDOW_WIDTH, SETTING_VALUE::WINDOW_HEIGHT));
}

void MyDirectX12::PostDraw()
{
#ifdef _DEBUG

	// imgui�`��
	ImGui::Render();
	ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());


#endif // _DEBUG



	EndBarrier();
	ExecuteCmd();
}

void MyDirectX12::PreDraw()
{
	// 1.���\�[�X�o���A��ύX
	BeginBarrier();

	// 2.��ʃN���A�R�}���h��������@
	ClearScreen();

	// 2.��ʃN���A�R�}���h�����܂�
	// 3.�`��R�}���h��������

	// �r���[�|�[�g�ƃV�U�[��`�̐ݒ�
	SetViewport();
	SetScissorrect();

	#ifdef _DEBUG

	// imgui�J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

#endif // _DEBUG

}

bool MyDirectX12::CreatDepthDesc()
{
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		SETTING_VALUE::WINDOW_WIDTH,
		SETTING_VALUE::WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	// ���\�[�X�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool MyDirectX12::InitImgui()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&imguiHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �X���b�v�`�F�[���̏����擾
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	if (ImGui::CreateContext() == nullptr) {
		assert(0);
		return false;
	}
	if (!ImGui_ImplWin32_Init(window->GetHWND())) {
		assert(0);
		return false;
	}
	if (!ImGui_ImplDX12_Init(
		GetDevice(),
		swcDesc.BufferCount,
		swcDesc.BufferDesc.Format,
		imguiHeap.Get(),
		imguiHeap->GetCPUDescriptorHandleForHeapStart(),
		imguiHeap->GetGPUDescriptorHandleForHeapStart()
	)) {
		assert(0);
		return false;
	}

	return true;


}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{	
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;

	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float& rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs;
	result.y = lhs.y + rhs;
	result.z = lhs.z + rhs;

	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;

	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const float& rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	result.z = lhs.z - rhs;

	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	DirectX::XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;

	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const float& rhs)
{
	DirectX::XMFLOAT4 result;
	result.x = lhs.x + rhs;
	result.y = lhs.y + rhs;
	result.z = lhs.z + rhs;
	result.w = lhs.w + rhs;

	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	DirectX::XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;

	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& lhs, const float& rhs)
{
	DirectX::XMFLOAT4 result;
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	result.z = lhs.z - rhs;
	result.w = lhs.w - rhs;

	return result;

}

DirectX::XMFLOAT4& operator-=(const DirectX::XMFLOAT4& lhs, const float& rhs)
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	DirectX::XMFLOAT4 result;
	result.x = lhs.x - rhs;
	result.y = lhs.y - rhs;
	result.z = lhs.z - rhs;
	result.w = lhs.w - rhs;

	return result;
}