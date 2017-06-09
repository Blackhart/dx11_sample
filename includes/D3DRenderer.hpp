#ifndef __D3D_RENDERER_HPP__
#define __D3D_RENDERER_HPP__

#include <windows.h>
#include <cstdint>
#include <memory>

#include "D3DFunctionalities.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "SceneObject.hpp"

extern bool const	VSYNC_ENABLED;
extern float const	SCREEN_FAR;
extern float const	SCREEN_NEAR;

enum eWindowMode;

class D3DRenderer
{
public:
	D3DRenderer() = default;
	D3DRenderer(D3DRenderer const&) = delete;
	D3DRenderer(D3DRenderer&&) = delete;
	~D3DRenderer() = default;
	D3DRenderer&	operator=(D3DRenderer const&) = delete;
	D3DRenderer&	operator=(D3DRenderer&&) = delete;
	bool	Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode);
	void	Uninitialize();
	bool	Render();
private:
	std::unique_ptr<D3DDevice>		__D3DInstance;
	std::unique_ptr<Camera>			__camera;
	std::unique_ptr<Mesh>			__mesh;
	std::unique_ptr<Material>		__material;
};

#endif