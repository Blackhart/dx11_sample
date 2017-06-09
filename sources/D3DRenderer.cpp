#include "../includes/D3DRenderer.hpp"
#include "../includes/Window.hpp"

bool const	VSYNC_ENABLED = true;

bool	D3DRenderer::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode)
{
	bool	lWNDMode = (pWNDMode == FULL_SCREEN);

	__D3DDevice = std::unique_ptr<D3DDevice>(new (std::nothrow) D3DDevice);
	if (__D3DDevice.get() == nullptr)
		return false;
	if (!__D3DDevice->Initialize(pWidth, pHeight, VSYNC_ENABLED, pHWND, lWNDMode))
	{
		MessageBox(pHWND, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	__camera = std::unique_ptr<Camera>(new (std::nothrow) Camera(0.01f, 1000.0f));
	if (__camera.get() == nullptr)
		return false;
	__camera->SetPosition(0.0f, 1.0f, 5.0f);

	__mesh = std::unique_ptr<Mesh>(new (std::nothrow) Mesh);
	if (__mesh.get() == nullptr)
		return false;
	if (!__mesh->Initialize())
	{
		MessageBox(pHWND, L"Could not initialize Model", L"Error", MB_OK);
		return false;
	}

	__material = std::unique_ptr<Material>(new (std::nothrow) Material);
	if (__material.get() == nullptr)
		return false;
	if (!__material->Initialize())
	{
		MessageBox(pHWND, L"Could not initialize Shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void	D3DRenderer::Uninitialize()
{
	if (__mesh.get() != nullptr)
		__mesh->Uninitialize();

	if (__material.get() != nullptr)
		__material->Uninitialize();

	if (__D3DDevice.get() != nullptr)
		__D3DDevice->Uninitialize();
}

bool	D3DRenderer::Render()
{
	__D3DDevice->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	if (!__camera->Render(*__mesh, *__material))
		return false;

	__D3DDevice->EndScene();

	return true;
}