#include "../includes/SampleEngine.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SampleEngine lEngine;
	if (!lEngine.Initialize())
	{
		lEngine.Uninitialize();
		return false;
	}
	lEngine.Run();
	lEngine.Uninitialize();
	return true;
}