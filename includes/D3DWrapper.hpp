#ifndef __D3D_WRAPPER_HPP__
#define __D3D_WRAPPER_HPP__

// Direct3D functionalities for setting up and drawing 3D graphics 
#pragma comment(lib, "d3d11.lib")
// Direct3D tools to interface with the hardware to obtain information about refresh rate, video card used, et so forth
#pragma comment(lib, "dxgi.lib")
// Contains functionalities for compiling hlsl shaders 
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdint>
#include <memory>

using namespace DirectX;

class D3DWrapper
{
public:
	D3DWrapper() = default;
	D3DWrapper(D3DWrapper const&) = delete;
	D3DWrapper(D3DWrapper&&) = delete;
	~D3DWrapper() = default;
	D3DWrapper&	operator=(D3DWrapper const&) = delete;
	D3DWrapper&	operator=(D3DWrapper&&) = delete;
	bool					Initialize(uint16_t const pWidth, uint16_t const pHeight, bool const pVSync, HWND const pHWND, bool const pFullScreen, float const pScreenFar, float const pScreenNear);
	void					Uninitialize();
	void					BeginScene(float const pRed, float const pGreen, float const pBlue, float const pAlpha);
	void					EndScene();
	ID3D11Device*			GetDevice() const;
	ID3D11DeviceContext*	GetDeviceContext() const;
	void					GetProjectionMatrix(XMMATRIX& pProjMatrix) const;
	void					GetWorldMatrix(XMMATRIX& pWorldMatrix) const;
	void					GetOrthoMatrix(XMMATRIX& pOrthoMatrix) const;
	void					GetVideoCardInfo(char* pCardName, int& pMemory) const;
private:
	bool					GetDeviceInfo(uint16_t const pWidth, uint16_t const pHeight, uint32_t& pFreshRateNum, uint32_t& pFreshRateDenom, uint64_t& pVideoCardNameLength);
	bool					InitializeDeviceWithSwapChain(uint16_t const pWidth, uint16_t const pHeight, uint32_t lFreshRateNum, uint32_t lFreshRateDenom, HWND const pHWND, bool const pFullScreen);
	bool					InitializeRenderTarget();
	bool					InitializeDepthBuffer(uint16_t const pWidth, uint16_t const pHeight);
	bool					InitializeRasterizer();
	void					InitializeViewport(uint16_t const pWidth, uint16_t const pHeight);
	void					InitializeMatrix(uint16_t const pWidth, uint16_t const pHeight, float const pScreenFar, float const pScreenNear);
private:
	bool						__vsync;
	int32_t						__videoCardMemory;
	char						__videoCardDescription[128];
	IDXGISwapChain*				__swapChain = nullptr;
	ID3D11Device*				__device = nullptr;
	ID3D11DeviceContext*		__deviceContext = nullptr;
	ID3D11RenderTargetView*		__renderTargetView = nullptr;
	ID3D11Texture2D*			__depthStencilBuffer = nullptr;
	ID3D11DepthStencilState*	__depthStencilState = nullptr;
	ID3D11DepthStencilView*		__depthStencilView = nullptr;
	ID3D11RasterizerState*		__rasterState = nullptr;
	XMMATRIX					__projectionMatrix;
	XMMATRIX					__worldMatrix;
	XMMATRIX					__orthoMatrix;
};

#endif