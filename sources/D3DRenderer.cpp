#include "../includes/D3DRenderer.hpp"
#include "../includes/Window.hpp"

bool const	VSYNC_ENABLED = true;
float const	SCREEN_FAR = 1000.0f;
float const	SCREEN_NEAR = 0.1f;

bool	D3DRenderer::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode)
{
	bool	lWNDMode = (pWNDMode == FULL_SCREEN);

	__D3DInstance = std::unique_ptr<D3DDevice>(new (std::nothrow) D3DDevice);
	if (__D3DInstance.get() == nullptr)
		return false;
	if (!__D3DInstance->Initialize(pWidth, pHeight, VSYNC_ENABLED, pHWND, lWNDMode, SCREEN_FAR, SCREEN_NEAR))
	{
		MessageBox(pHWND, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	__camera = std::unique_ptr<Camera>(new (std::nothrow) Camera);
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

	if (__D3DInstance.get() != nullptr)
		__D3DInstance->Uninitialize();
}

bool	D3DRenderer::Render()
{
	XMMATRIX	lWorldMatrix;
	XMMATRIX	lViewMatrix;
	XMMATRIX	lProjMatrix;

	__D3DInstance->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	__camera->Render();

	__D3DInstance->GetWorldMatrix(lWorldMatrix);
	__camera->GetViewMatrix(lViewMatrix);
	__D3DInstance->GetProjectionMatrix(lProjMatrix);

	SceneObject lsp(__mesh.get(), __material.get());

	__mesh->SentToGPU();

	if (!__material->SentToGPU(__mesh->GetIndexCount(), lWorldMatrix, lViewMatrix, lProjMatrix))
		return false;

	__D3DInstance->EndScene();

	return true;
}