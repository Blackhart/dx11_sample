#include "../includes/D3DInstance.hpp"

D3DInstance::D3DInstance() :
	__swapChain{ nullptr },
	__device{ nullptr },
	__deviceContext{ nullptr },
	__renderTargetView{ nullptr },
	__depthStencilBuffer{ nullptr },
	__depthStencilState{ nullptr },
	__depthStencilView{ nullptr },
	__rasterState{ nullptr }
{

}

bool	D3DInstance::Initialize(uint16_t const pWidth, uint16_t const pHeight, bool const pVSync, HWND const pHWND, bool const pFullScreen, float const pScreenFar, float const pScreenNear)
{
	uint32_t	lFreshRateNum;
	uint32_t	lFreshRateDenom;
	uint64_t	lVideoCardNameLength;

	__vsync = pVSync;

	if (!GetDeviceInfo(pWidth, pHeight, lFreshRateNum, lFreshRateDenom, lVideoCardNameLength))
		return false;

	if (!InitializeDeviceWithSwapChain(pWidth, pHeight, lFreshRateNum, lFreshRateDenom, pHWND, pFullScreen))
		return false;

	if (!InitializeRenderTarget())
		return false;

	if (!InitializeDepthBuffer(pWidth, pHeight))
		return false;

	if (!InitializeRasterizer())
		return false;

	InitializeViewport(pWidth, pHeight);

	InitializeMatrix(pWidth, pHeight, pScreenFar, pScreenNear);

	return true;
}

bool	D3DInstance::GetDeviceInfo(uint16_t const pWidth, uint16_t const pHeight, uint32_t& pFreshRateNum, uint32_t& pFreshRateDenom, uint64_t& pVideoCardNameLength)
{
	HRESULT				lResult;
	IDXGIFactory*		lpFactory;
	IDXGIAdapter*		lpAdapter;
	IDXGIOutput*		lpOutput;
	DXGI_MODE_DESC*		lpModeDesc;
	DXGI_ADAPTER_DESC	lAdapterDesc;
	uint32_t			lNumModes;

	lResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&lpFactory);
	if (FAILED(lResult))
		goto Quit;
	lResult = lpFactory->EnumAdapters(0, (IDXGIAdapter**)&lpAdapter);
	if (FAILED(lResult))
		goto Release_Factory;
	lResult = lpAdapter->EnumOutputs(0, &lpOutput);
	if (FAILED(lResult))
		goto Release_Adapter;
	lResult = lpOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &lNumModes, nullptr);
	if (FAILED(lResult))
		goto Release_Output;
	lpModeDesc = new DXGI_MODE_DESC[lNumModes];
	if (lpModeDesc == nullptr)
		goto Release_Output;
	lResult = lpOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &lNumModes, lpModeDesc);
	if (FAILED(lResult))
		goto Release_ModeDesc;

	for (uint32_t i = 0; i < lNumModes; ++i)
	{
		if (lpModeDesc[i].Width == pWidth && lpModeDesc[i].Height == pHeight)
		{
			pFreshRateNum = lpModeDesc[i].RefreshRate.Numerator;
			pFreshRateDenom = lpModeDesc[i].RefreshRate.Denominator;
		}
	}

	lResult = lpAdapter->GetDesc(&lAdapterDesc);
	if (FAILED(lResult))
		goto Release_ModeDesc;
	__videoCardMemory = (int32_t)(lAdapterDesc.DedicatedVideoMemory / 1024 / 1024);
	if (wcstombs_s(&pVideoCardNameLength, __videoCardDescription, 128, lAdapterDesc.Description, 128) != 0)
		goto Release_ModeDesc;

	delete[] lpModeDesc;
	lpOutput->Release();
	lpAdapter->Release();
	lpFactory->Release();

	return true;

	// ~~~~~ ERROR ~~~~~

Release_ModeDesc:
	delete[] lpModeDesc;
Release_Output:
	lpOutput->Release();
Release_Adapter:
	lpAdapter->Release();
Release_Factory:
	lpFactory->Release();
Quit:
	return false;
}

bool	D3DInstance::InitializeDeviceWithSwapChain(uint16_t const pWidth, uint16_t const pHeight, uint32_t const lFreshRateNum, uint32_t const lFreshRateDenom, HWND const pHWND, bool const pFullScreen)
{
	HRESULT					lResult;
	D3D_FEATURE_LEVEL		lFeatureLvl;
	DXGI_SWAP_CHAIN_DESC	lSwapChainDesc;

	ZeroMemory(&lSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	lSwapChainDesc.BufferCount = 1;
	lSwapChainDesc.BufferDesc.Width = pWidth;
	lSwapChainDesc.BufferDesc.Height = pHeight;
	lSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (__vsync)
	{
		lSwapChainDesc.BufferDesc.RefreshRate.Numerator = lFreshRateNum;
		lSwapChainDesc.BufferDesc.RefreshRate.Denominator = lFreshRateDenom;
	}
	else
	{
		lSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		lSwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	}
	lSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	lSwapChainDesc.OutputWindow = pHWND;
	lSwapChainDesc.SampleDesc.Count = 1;
	lSwapChainDesc.SampleDesc.Quality = 0;
	if (pFullScreen)
		lSwapChainDesc.Windowed = false;
	else
		lSwapChainDesc.Windowed = true;
	lSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	lSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	lSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	lSwapChainDesc.Flags = 0;

	lFeatureLvl = D3D_FEATURE_LEVEL_11_0;

	lResult = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &lFeatureLvl, 1, D3D11_SDK_VERSION, &lSwapChainDesc, &__swapChain, &__device, nullptr, &__deviceContext);
	if (FAILED(lResult))
		return false;

	return true;
}

bool	D3DInstance::InitializeRenderTarget()
{
	HRESULT				lResult;
	ID3D11Texture2D*	lpBackBuffer;

	lResult = __swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&lpBackBuffer);
	if (FAILED(lResult))
		goto Quit;

	lResult = __device->CreateRenderTargetView(lpBackBuffer, nullptr, &__renderTargetView);
	if (FAILED(lResult))
		goto Release_BackBuffer;

	lpBackBuffer->Release();

	return true;

	// ~~~~~ ERROR ~~~~~

Release_BackBuffer:
	lpBackBuffer->Release();
Quit:
	return false;
}

bool	D3DInstance::InitializeDepthBuffer(uint16_t const pWidth, uint16_t const pHeight)
{
	HRESULT							lResult;
	D3D11_TEXTURE2D_DESC			lDepthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC		lDepthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC	lDepthStencilViewDesc;

	ZeroMemory(&lDepthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	lDepthBufferDesc.Width = pWidth;
	lDepthBufferDesc.Height = pHeight;
	lDepthBufferDesc.MipLevels = 1;
	lDepthBufferDesc.ArraySize = 1;
	lDepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	lDepthBufferDesc.SampleDesc.Count = 1;
	lDepthBufferDesc.SampleDesc.Quality = 0;
	lDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	lDepthBufferDesc.CPUAccessFlags = 0;
	lDepthBufferDesc.MiscFlags = 0;
	lResult = __device->CreateTexture2D(&lDepthBufferDesc, nullptr, &__depthStencilBuffer);
	if (FAILED(lResult))
		return false;

	ZeroMemory(&lDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	lDepthStencilDesc.DepthEnable = true;
	lDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	lDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	lDepthStencilDesc.StencilEnable = true;
	lDepthStencilDesc.StencilReadMask = 0xFF;
	lDepthStencilDesc.StencilWriteMask = 0xFF;
	lDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	lDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	lDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	lDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	lDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	lDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	lDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	lDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	lResult = __device->CreateDepthStencilState(&lDepthStencilDesc, &__depthStencilState);
	if (FAILED(lResult))
		return false;

	__deviceContext->OMSetDepthStencilState(__depthStencilState, 1);

	ZeroMemory(&lDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	lDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	lDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	lDepthStencilViewDesc.Texture2D.MipSlice = 0;
	lResult = __device->CreateDepthStencilView(__depthStencilBuffer, &lDepthStencilViewDesc, &__depthStencilView);
	if (FAILED(lResult))
		return false;

	__deviceContext->OMSetRenderTargets(1, &__renderTargetView, __depthStencilView);

	return true;
}

bool	D3DInstance::InitializeRasterizer()
{
	HRESULT					lResult;
	D3D11_RASTERIZER_DESC	lRasterDesc;

	lRasterDesc.AntialiasedLineEnable = false;
	lRasterDesc.CullMode = D3D11_CULL_BACK;
	lRasterDesc.DepthBias = 0;
	lRasterDesc.DepthBiasClamp = 0.0f;
	lRasterDesc.DepthClipEnable = true;
	lRasterDesc.FillMode = D3D11_FILL_SOLID;
	lRasterDesc.FrontCounterClockwise = false;
	lRasterDesc.MultisampleEnable = false;
	lRasterDesc.ScissorEnable = false;
	lRasterDesc.SlopeScaledDepthBias = 0.0f;
	lResult = __device->CreateRasterizerState(&lRasterDesc, &__rasterState);
	if (FAILED(lResult))
		return false;

	__deviceContext->RSSetState(__rasterState);

	return true;
}

void	D3DInstance::InitializeViewport(uint16_t const pWidth, uint16_t const pHeight)
{
	D3D11_VIEWPORT	lViewport;

	lViewport.Width = pWidth;
	lViewport.Height = pHeight;
	lViewport.MinDepth = 0.0f;
	lViewport.MaxDepth = 1.0f;
	lViewport.TopLeftX = 0.0f;
	lViewport.TopLeftY = 0.0f;

	__deviceContext->RSSetViewports(1, &lViewport);
}

void	D3DInstance::InitializeMatrix(uint16_t const pWidth, uint16_t const pHeight, float const pScreenFar, float const pScreenNear)
{
	float	lFieldOfView;
	float	lScreenAspect;

	lFieldOfView = 3.141592654f / 4.0f;
	lScreenAspect = (float)pWidth / (float)pHeight;

	__projectionMatrix = XMMatrixPerspectiveFovLH(lFieldOfView, lScreenAspect, pScreenNear, pScreenFar);
	__worldMatrix = XMMatrixIdentity();
	__orthoMatrix = XMMatrixOrthographicLH((float)pWidth, (float)pHeight, pScreenNear, pScreenFar);
}

void	D3DInstance::Uninitialize()
{
	if (__swapChain)
		__swapChain->SetFullscreenState(false, nullptr);

	if (__rasterState)
		__rasterState->Release();

	if (__depthStencilView)
		__depthStencilView->Release();

	if (__depthStencilState)
		__depthStencilState->Release();

	if (__depthStencilBuffer)
		__depthStencilBuffer->Release();

	if (__renderTargetView)
		__renderTargetView->Release();

	if (__deviceContext)
		__deviceContext->Release();

	if (__device)
		__device->Release();

	if (__swapChain)
		__swapChain->Release();
}

void	D3DInstance::BeginScene(float const pRed, float const pGreen, float const pBlue, float const pAlpha)
{
	float	lClearColor[4];

	lClearColor[0] = pRed; lClearColor[1] = pGreen; lClearColor[2] = pBlue; lClearColor[3] = pAlpha;

	__deviceContext->ClearRenderTargetView(__renderTargetView, lClearColor);
	__deviceContext->ClearDepthStencilView(__depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void	D3DInstance::EndScene()
{
	if (__vsync)
		__swapChain->Present(1, 0);
	else
		__swapChain->Present(0, 0);
}

ID3D11Device*	D3DInstance::GetDevice() const
{
	return __device;
}

ID3D11DeviceContext*	D3DInstance::GetDeviceContext() const
{
	return __deviceContext;
}

void	D3DInstance::GetProjectionMatrix(XMMATRIX& pProjMatrix) const
{
	pProjMatrix = __projectionMatrix;
}

void	D3DInstance::GetWorldMatrix(XMMATRIX& pWorldMatrix) const
{
	pWorldMatrix = __worldMatrix;
}

void	D3DInstance::GetOrthoMatrix(XMMATRIX& pOrthoMatrix) const
{
	pOrthoMatrix = __orthoMatrix;
}

void	D3DInstance::GetVideoCardInfo(char* pCardName, int& pMemory) const
{
	strcpy_s(pCardName, 128, __videoCardDescription);
	pMemory = __videoCardMemory;
}