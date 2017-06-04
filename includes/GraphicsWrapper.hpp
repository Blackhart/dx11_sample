#ifndef __GRAPHICS_WRAPPER_HPP__
#define __GRAPHICS_WRAPPER_HPP__

#include <windows.h>
#include <cstdint>
#include <memory>

#include "D3DWrapper.hpp"
#include "../includes/Camera.hpp"
#include "../includes/Model.hpp"
#include "../includes/Shader.hpp"

extern bool const	VSYNC_ENABLED;
extern float const	SCREEN_FAR;
extern float const	SCREEN_NEAR;

enum eWindowMode;

class GraphicsWrapper
{
public:
	GraphicsWrapper() = default;
	GraphicsWrapper(GraphicsWrapper const&) = delete;
	GraphicsWrapper(GraphicsWrapper&&) = delete;
	~GraphicsWrapper() = default;
	GraphicsWrapper&	operator=(GraphicsWrapper const&) = delete;
	GraphicsWrapper&	operator=(GraphicsWrapper&&) = delete;
	bool	Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode);
	void	Uninitialize();
	bool	Render();
private:
	std::unique_ptr<D3DWrapper>	__D3DWrapper;
	std::unique_ptr<Camera>		__camera;
	std::unique_ptr<Model>		__model;
	std::unique_ptr<Shader>		__shader;
};

#endif