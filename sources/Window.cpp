#include "../includes/Window.hpp"

Window*	WindowInst = nullptr;

Window::Window() :
	__windowWidth{ 0 },
	__windowHeight{ 0 }
{

}

bool	Window::Initialize()
{
	InitializeWindows();

	__input = std::unique_ptr<InputWrapper>(new (std::nothrow) InputWrapper{});
	if (__input.get() == nullptr)
		return false;
	__input->Initialize();

	return true;
}

void	Window::Uninitialize()
{
	UninitializeWindows();
}

bool	Window::Run()
{
	MSG	lMsg;

	ZeroMemory(&lMsg, sizeof(MSG));
	if (PeekMessage(&lMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&lMsg);
		DispatchMessage(&lMsg);
	}

	if (lMsg.message == WM_QUIT)
		return false;
	else if (__input->IsKeyDown(VK_ESCAPE))
		return false;
	return true;
}

LRESULT CALLBACK Window::MessageHandler(HWND const hwnd, UINT const umsg, WPARAM const wparam, LPARAM const lparam)
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

void	Window::InitializeWindows()
{
	WNDCLASSEX	lWNDCLASSEX;
	DEVMODE		lDEVMODE;
	uint16_t	posx = 0;
	uint16_t	posy = 0;

	WindowInst = this;

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

	__windowWidth = GetSystemMetrics(SM_CXSCREEN);
	__windowHeight = GetSystemMetrics(SM_CYSCREEN);

	if (__windowMode == FULL_SCREEN)
	{
		memset(&lDEVMODE, 0, sizeof(lDEVMODE));
		lDEVMODE.dmSize = sizeof(lDEVMODE);
		lDEVMODE.dmPelsWidth = (unsigned long)__windowWidth;
		lDEVMODE.dmPelsHeight = (unsigned long)__windowHeight;
		lDEVMODE.dmBitsPerPel = 32;
		lDEVMODE.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&lDEVMODE, CDS_FULLSCREEN);

		posx = 0;
		posy = 0;
	}
	else
	{
		__windowWidth = 800;
		__windowHeight = 600;

		posx = (GetSystemMetrics(SM_CXSCREEN) - __windowWidth) / 2;
		posy = (GetSystemMetrics(SM_CYSCREEN) - __windowHeight) / 2;
	}

	__hwnd = CreateWindowEx(WS_EX_APPWINDOW, __appName, __appName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posx, posy, __windowWidth, __windowHeight, NULL, NULL, __hinstance, NULL);

	ShowWindow(__hwnd, SW_SHOW);
	SetForegroundWindow(__hwnd);
	SetFocus(__hwnd);
	ShowCursor(false);
}

void	Window::UninitializeWindows()
{
	ShowCursor(true);

	if (CDS_FULLSCREEN)
		ChangeDisplaySettings(nullptr, 0);
	
	DestroyWindow(__hwnd);
	
	UnregisterClass(__appName, __hinstance);
	
	__hinstance = nullptr;
	__appName = nullptr;
}

LRESULT CALLBACK WndProc(HWND const hwnd, UINT const umessage, WPARAM const wparam, LPARAM const lparam)
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
			return WindowInst->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

uint16_t	Window::GetWindowWidth() const
{
	return __windowWidth;
}

uint16_t	Window::GetWindowHeight() const
{
	return __windowHeight;
}

HWND	Window::GetWindow() const
{
	return __hwnd;
}

WindowMode	Window::GetWindowMode() const
{
	return __windowMode;
}