#ifndef __GRAPHICS_WRAPPER_HPP__
#define __GRAPHICS_WRAPPER_HPP__

#include <windows.h>
#include <cstdint>
#include <memory>

#include "D3DInstance.hpp"
#include "../includes/Camera.hpp"
#include "../includes/Geometry.hpp"
#include "../includes/Shader.hpp"
#include "../includes/Mesh.hpp"

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
	std::unique_ptr<D3DInstance>	__D3DInstance;
	std::unique_ptr<Camera>			__camera;
	std::unique_ptr<Geometry>		__model;
	std::unique_ptr<Shader>			__shader;
};

#endif