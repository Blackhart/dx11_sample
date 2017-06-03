#include "../includes/GraphicsWrapper.hpp"

bool const	VSYNC_ENABLED = true;
float const	SCREEN_DEPTH = 1000.0f;
float const	SCREEN_NEAR = 0.1f;

bool	GraphicsWrapper::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND)
{
	return true;
}

void	GraphicsWrapper::Uninitialize()
{

}

bool	GraphicsWrapper::Render()
{
	return true;
}