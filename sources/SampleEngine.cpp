#include "../includes/SampleEngine.hpp"

bool	SampleEngine::Initialize()
{
	uint16_t lWidth = 0;
	uint16_t lHeight = 0;

	InitializeWindows(lWidth, lHeight);

	__input = std::unique_ptr<InputWrapper>(new InputWrapper{});
	__input->Initialize();
	__graphics = std::unique_ptr<GraphicsWrapper>(new GraphicsWrapper{});
	//__graphics->Initialize();

	return true;
}

void	SampleEngine::Uninitialize()
{
	__input->Uninitialize();
	//__graphics->Uninitialize();
	UninitializeWindows();
}

void	SampleEngine::Run()
{
	MSG	lmsg;

	ZeroMemory(&lmsg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&lmsg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&lmsg);
			DispatchMessage(&lmsg);
		}

		if (lmsg.message == WM_QUIT)
			break;
		else if (!Render())
			break;
	}
}

bool	SampleEngine::Render()
{
	if (__input->IsKeyDown(VK_ESCAPE))
		return false;

//	if (__graphics->Render())
//		return false;

	return true;
}

LRESULT CALLBACK SampleEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			__input->KeyDown((unsigned int)wparam);
			return 0;
		}

		case WM_KEYUP:
		{
			__input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void	SampleEngine::InitializeWindows(uint16_t& pWidth, uint16_t& pHeight)
{
	WNDCLASSEX lWNDCLASSEX;
	DEVMODE	lDEVMODE;
	uint16_t posx = 0;
	uint16_t posy = 0;

	SampleEngineInst = this;
	__hinstance = GetModuleHandle(nullptr);
	__appName = L"Sample Engine";

	lWNDCLASSEX.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	lWNDCLASSEX.lpfnWndProc = WndProc;
	lWNDCLASSEX.cbClsExtra = 0;
	lWNDCLASSEX.cbWndExtra = 0;
	lWNDCLASSEX.hInstance = __hinstance;
	lWNDCLASSEX.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	lWNDCLASSEX.hIconSm = lWNDCLASSEX.hIcon;
	lWNDCLASSEX.hCursor = LoadCursor(NULL, IDC_ARROW);
	lWNDCLASSEX.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	lWNDCLASSEX.lpszMenuName = NULL;
	lWNDCLASSEX.lpszClassName = __appName;
	lWNDCLASSEX.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&lWNDCLASSEX);

	pWidth = GetSystemMetrics(SM_CXSCREEN);
	pHeight = GetSystemMetrics(SM_CYSCREEN);

	if (__windowMode == FULL_SCREEN)
	{
		memset(&lDEVMODE, 0, sizeof(lDEVMODE));
		lDEVMODE.dmSize = sizeof(lDEVMODE);
		lDEVMODE.dmPelsWidth = (unsigned long)pWidth;
		lDEVMODE.dmPelsHeight = (unsigned long)pHeight;
		lDEVMODE.dmBitsPerPel = 32;
		lDEVMODE.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&lDEVMODE, CDS_FULLSCREEN);

		posx = 0;
		posy = 0;
	}
	else
	{
		pWidth = 800;
		pHeight = 600;

		posx = (GetSystemMetrics(SM_CXSCREEN) - pWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - pHeight) / 2;
	}

	__hwnd = CreateWindowEx(WS_EX_APPWINDOW, __appName, __appName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posx, posy, pWidth, pHeight, NULL, NULL, __hinstance, NULL);

	ShowWindow(__hwnd, SW_SHOW);
	SetForegroundWindow(__hwnd);
	SetFocus(__hwnd);
	ShowCursor(false);
}

void	SampleEngine::UninitializeWindows()
{
	ShowCursor(true);

	if (CDS_FULLSCREEN)
		ChangeDisplaySettings(nullptr, 0);
	
	DestroyWindow(__hwnd);
	
	UnregisterClass(__appName, __hinstance);
	
	__hinstance = nullptr;
	__appName = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return SampleEngineInst->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}