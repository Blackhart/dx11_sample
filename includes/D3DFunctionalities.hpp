#ifndef __D3D_FUNCTIONALITIES_HPP__
#define __D3D_FUNCTIONALITIES_HPP__

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

// ~~~~~ Dcl(PUBLIC) ~~~~~

class D3DDevice
{
public:
	D3DDevice();
	D3DDevice(D3DDevice const&) = delete;
	D3DDevice(D3DDevice&&) = delete;
	~D3DDevice() = default;
	D3DDevice&	operator=(D3DDevice const&) = delete;
	D3DDevice&	operator=(D3DDevice&&) = delete;
	bool					Initialize(uint16_t const pWidth, uint16_t const pHeight, bool const pVSync, HWND const pHWND, bool const pFullScreen);
	void					Uninitialize();
	void					BeginScene(float const pRed, float const pGreen, float const pBlue, float const pAlpha);
	void					EndScene();
	ID3D11Device*			GetDevice() const;
	ID3D11DeviceContext*	GetDeviceContext() const;
	void					GetVideoCardInfo(char* pCardName, int& pMemory) const;
private:
	bool					GetDeviceInfo(uint16_t const pWidth, uint16_t const pHeight, uint32_t& pFreshRateNum, uint32_t& pFreshRateDenom, uint64_t& pVideoCardNameLength);
	bool					InitializeDeviceWithSwapChain(uint16_t const pWidth, uint16_t const pHeight, uint32_t const lFreshRateNum, uint32_t const lFreshRateDenom, HWND const pHWND, bool const pFullScreen);
	bool					InitializeRenderTarget();
	bool					InitializeDepthBuffer(uint16_t const pWidth, uint16_t const pHeight);
	bool					InitializeRasterizer();
	void					InitializeViewport(uint16_t const pWidth, uint16_t const pHeight);
private:
	bool						__vsync;
	int32_t						__videoCardMemory;
	char						__videoCardDescription[128];
	IDXGISwapChain*				__swapChain;
	ID3D11Device*				__device;
	ID3D11DeviceContext*		__deviceContext;
	ID3D11RenderTargetView*		__renderTargetView;
	ID3D11Texture2D*			__depthStencilBuffer;
	ID3D11DepthStencilState*	__depthStencilState;
	ID3D11DepthStencilView*		__depthStencilView;
	ID3D11RasterizerState*		__rasterState;
};

extern D3DDevice*	D3DInst;

extern bool	CreateBuffer(ID3D11Buffer** pBuffer, UINT const pBytesWidth, D3D11_USAGE const pUsage, UINT const pBindFlags, UINT const pCPUAccessFlags, UINT const pMiscFlags, UINT const pStructueBytesStride, void const* pData, UINT const pDataMemPitch, UINT const pDataMemSlicePitch);

#endif