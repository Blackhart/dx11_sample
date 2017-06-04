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
	Shader();
	Shader(Shader const&) = delete;
	Shader(Shader&&) = delete;
	~Shader() = default;
	Shader&	operator=(Shader const&) = delete;
	Shader&	operator=(Shader&&) = delete;
	bool	Initialize(ID3D11Device* const pDevice, HWND const pHWND);
	void	Uninitialize();
	bool	Render(ID3D11DeviceContext* const pDeviceContext, int const pIndexCount, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjectionMatrix);
private:
	void	OutputShaderErrorMessage(ID3D10Blob* const pErrorMsg, HWND const pHWND, WCHAR const* const pShaderFilename);
	bool	SetShaderParameters(ID3D11DeviceContext* const pDeviceContext, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjMatrix);
	bool	CompileShader(WCHAR const* const pShaderFilename, char const* const pEntryPoint, char const* const pShaderTarget, ID3DBlob** pShaderBuffer, HWND const pHWND);
	bool	InitializeVertexInputData(ID3D11Device* const pDevice, ID3DBlob* const pVertexShader);
	bool	InitializeMatrixBuffer(ID3D11Device* const pDevice);
private:
	ID3D11VertexShader*	__vertexShader;
	ID3D11PixelShader*	__pixelShader;
	ID3D11InputLayout*	__inputLayout;
	ID3D11Buffer*		__matrixBuffer;
};

#endif