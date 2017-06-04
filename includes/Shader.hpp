#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <cstdint>

using namespace DirectX;

class Shader
{
private:
	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Shader() = default;
	Shader(Shader const&) = delete;
	Shader(Shader&&) = delete;
	~Shader() = default;
	Shader&	operator=(Shader const&) = delete;
	Shader&	operator=(Shader&&) = delete;
	bool	Initialize(ID3D11Device* const pDevice, HWND pHWND);
	void	Uninitialize();
	bool	Render(ID3D11DeviceContext* pDeviceContext, int pIndexCount, XMMATRIX pWorldMatrix, XMMATRIX pViewMatrix, XMMATRIX pProjectionMatrix);
private:
	void	OutputShaderErrorMessage(ID3D10Blob* const pErrorMsg, HWND pHWND, WCHAR const* const pShaderFilename);
	bool	SetShaderParameters(ID3D11DeviceContext* const pDeviceContext, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjMatrix);
	bool	CompileShader(WCHAR* const pShaderFilename, ID3DBlob** pShaderBuffer, HWND pHWND);
	bool	InitializeVertexInputData(ID3D11Device* const pDevice, ID3DBlob* pVertexShader);
	bool	InitializeMatrixBuffer(ID3D11Device* const pDevice);
private:
	ID3D11VertexShader*	__vertexShader = nullptr;
	ID3D11PixelShader*	__pixelShader = nullptr;
	ID3D11InputLayout*	__inputLayout = nullptr;
	ID3D11Buffer*		__matrixBuffer = nullptr;
};

#endif