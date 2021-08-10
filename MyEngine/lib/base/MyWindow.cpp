#include "MyWindow.h"
#include<Windows.h>


MyWindow::MyWindow()
{

}
MyWindow::~MyWindow()
{
	Release();
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E���j�󂳂ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;

	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // �W���̏������s��
}


void MyWindow::Initialize(const char* windowName)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = windowName; // �E�B���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���ݒ�

	// �E�B���h�E�N���X��os�ɓo�^
	RegisterClassEx(&w);
	// �E�B���h�E�T�C�Y{�����W�@�����W�@�����@�c��}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName, //�@�N���X��
		windowName,   // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW, //	�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,  // �\�������W
		CW_USEDEFAULT,  // �\�������W
		wrc.right - wrc.left,  // �E�B���h�E���� 
		wrc.bottom - wrc.top,  // �E�B���h�E�c��
		nullptr,               // �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		w.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				//	�I�v�V����


	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);

}

HWND MyWindow::GetHWND()
{
	return hwnd;
}
void MyWindow::Release()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);
}

const int MyWindow::GetWidth()
{
	return window_width;
}
const int MyWindow::GetHeight()
{
	return window_height;
}

WNDCLASSEX MyWindow::GetConfig()
{
	return w;
}