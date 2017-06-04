#include "../includes/GraphicsWrapper.hpp"
#include "../includes/SampleEngine.hpp"

bool const	VSYNC_ENABLED = true;
float const	SCREEN_FAR = 1000.0f;
float const	SCREEN_NEAR = 0.1f;

bool	GraphicsWrapper::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode)
{
	bool	lWNDMode = (pWNDMode == FULL_SCREEN);

	__D3DWrapper = std::unique_ptr<D3DWrapper>(new (std::nothrow) D3DWrapper);
	if (__D3DWrapper.get() == nullptr)
		return false;
	if (!__D3DWrapper->Initialize(pWidth, pHeight, VSYNC_ENABLED, pHWND, lWNDMode, SCREEN_FAR, SCREEN_NEAR))
	{
		MessageBox(pHWND, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	__camera = std::unique_ptr<Camera>(new (std::nothrow) Camera);
	if (__camera.get() == nullptr)
		return false;
	__camera->SetPosition(0.0f, 0.0f, -10.5f);

	__model = std::unique_ptr<Model>(new (std::nothrow) Model);
	if (__model.get() == nullptr)
		return false;
	if (!__model->Initialize(__D3DWrapper->GetDevice()))
	{
		MessageBox(pHWND, L"Could not initialize Model", L"Error", MB_OK);
		return false;
	}

	__shader = std::unique_ptr<Shader>(new (std::nothrow) Shader);
	if (__shader.get() == nullptr)
		return false;
	if (!__shader->Initialize(__D3DWrapper->GetDevice(), pHWND))
	{
		MessageBox(pHWND, L"Could not initialize Shader", L"Error", MB_OK);
		return false;
	}

	return true;
}

void	GraphicsWrapper::Uninitialize()
{
	if (__model.get() != nullptr)
		__model->Uninitialize();

	if (__shader.get() != nullptr)
		__shader->Uninitialize();

	if (__D3DWrapper.get() != nullptr)
		__D3DWrapper->Uninitialize();
}

bool	GraphicsWrapper::Render()
{
	XMMATRIX	lWorldMatrix;
	XMMATRIX	lViewMatrix;
	XMMATRIX	lProjMatrix;

	__D3DWrapper->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	__camera->Render();

	__D3DWrapper->GetWorldMatrix(lWorldMatrix);
	__camera->GetViewMatrix(lViewMatrix);
	__D3DWrapper->GetProjectionMatrix(lProjMatrix);

	__model->Render(__D3DWrapper->GetDeviceContext());

	if (!__shader->Render(__D3DWrapper->GetDeviceContext(), __model->GetIndexCount(), lWorldMatrix, lViewMatrix, lProjMatrix))
		return false;

	__D3DWrapper->EndScene();

	return true;
}