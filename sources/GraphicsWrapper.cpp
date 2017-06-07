#include "../includes/GraphicsWrapper.hpp"
#include "../includes/SampleEngine.hpp"

bool const	VSYNC_ENABLED = true;
float const	SCREEN_FAR = 1000.0f;
float const	SCREEN_NEAR = 0.1f;

bool	GraphicsWrapper::Initialize(uint16_t const pWidth, uint16_t const pHeight, HWND const pHWND, eWindowMode const pWNDMode)
{
	bool	lWNDMode = (pWNDMode == FULL_SCREEN);

	__D3DInstance = std::unique_ptr<D3DInstance>(new (std::nothrow) D3DInstance);
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
	__camera->SetPosition(0.0f, 0.0f, 5.0f);

	__model = std::unique_ptr<Geometry>(new (std::nothrow) Geometry);
	if (__model.get() == nullptr)
		return false;
	if (!__model->Initialize(__D3DInstance->GetDevice()))
	{
		MessageBox(pHWND, L"Could not initialize Model", L"Error", MB_OK);
		return false;
	}

	__shader = std::unique_ptr<Shader>(new (std::nothrow) Shader);
	if (__shader.get() == nullptr)
		return false;
	if (!__shader->Initialize(__D3DInstance->GetDevice(), pHWND))
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

	if (__D3DInstance.get() != nullptr)
		__D3DInstance->Uninitialize();
}

bool	GraphicsWrapper::Render()
{
	XMMATRIX	lWorldMatrix;
	XMMATRIX	lViewMatrix;
	XMMATRIX	lProjMatrix;

	__D3DInstance->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	__camera->Render();

	__D3DInstance->GetWorldMatrix(lWorldMatrix);
	__camera->GetViewMatrix(lViewMatrix);
	__D3DInstance->GetProjectionMatrix(lProjMatrix);

	ID3D11Buffer* const lVertexBuffer = __model->GetVertexBuffer();
	ID3D11Buffer* const lIndexBuffer = __model->GetIndexBuffer();
	UINT lBufferSize = __model->GetBufferSize();
	UINT lOffset = 0;
	__D3DInstance->GetDeviceContext()->IASetVertexBuffers(0, 1, &lVertexBuffer, &lBufferSize, &lOffset);
	__D3DInstance->GetDeviceContext()->IASetIndexBuffer(lIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	__D3DInstance->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!__shader->Render(__D3DInstance->GetDeviceContext(), __model->GetIndexCount(), lWorldMatrix, lViewMatrix, lProjMatrix))
		return false;

	__D3DInstance->EndScene();

	return true;
}