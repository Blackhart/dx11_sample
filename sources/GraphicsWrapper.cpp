#include "../includes/GraphicsWrapper.hpp"
#include "../includes/SampleEngine.hpp"

bool const	VSYNC_ENABLED = true;
float const	SCREEN_FAR = 1000.0f;
float const	SCREEN_NEAR = 0.1f;

bool	GraphicsWrapper::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode)
{
	bool lWNDMode = (pWNDMode == FULL_SCREEN);
	if (!__D3DWrapper.Initialize(pWidth, pHeight, VSYNC_ENABLED, pHWND, lWNDMode, SCREEN_FAR, SCREEN_NEAR))
	{
		MessageBox(pHWND, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	return true;
}

void	GraphicsWrapper::Uninitialize()
{
	__D3DWrapper.Uninitialize();
}

bool	GraphicsWrapper::Render()
{
	__D3DWrapper.BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	__D3DWrapper.EndScene();
	return true;
}