#include "../includes/Window.hpp"
#include "../includes/D3DRenderer.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Window		lWindow;
	D3DRenderer	lRenderer;

	if (!lWindow.Initialize())
		return false;
	if (!lRenderer.Initialize(lWindow.GetWindowWidth(), lWindow.GetWindowHeight(), lWindow.GetWindow(), lWindow.GetWindowMode()))
		return false;

	while (lWindow.Run() && lRenderer.Render());

	lRenderer.Uninitialize();
	lWindow.Uninitialize();
	return true;
}