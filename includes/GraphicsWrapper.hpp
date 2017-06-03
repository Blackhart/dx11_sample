#ifndef __GRAPHICS_WRAPPER_HPP__
#define __GRAPHICS_WRAPPER_HPP__

#include <windows.h>
#include <cstdint>

extern bool const	VSYNC_ENABLED;
extern float const	SCREEN_DEPTH;
extern float const	SCREEN_NEAR;

class GraphicsWrapper
{
public:
	GraphicsWrapper() = default;
	GraphicsWrapper(GraphicsWrapper const&) = delete;
	GraphicsWrapper(GraphicsWrapper&&) = delete;
	~GraphicsWrapper() = default;
	GraphicsWrapper&	operator=(GraphicsWrapper const&) = delete;
	GraphicsWrapper&	operator=(GraphicsWrapper&&) = delete;
	bool	Initialize(uint16_t const, uint16_t const, HWND const);
	void	Uninitialize();
	bool	Render();
};

#endif