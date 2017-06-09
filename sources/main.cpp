#include "../includes/Window.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Window lWindow;
	if (!lWindow.Initialize())
		return false;

	while (lWindow.Run())
	{

	}

	lWindow.Uninitialize();
	return true;
}