#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <cstdint>

using namespace DirectX;

class Material
{
private:
	struct MatrixBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	Material();
	Material(Material const&) = delete;
	Material(Material&&) = delete;
	~Material() = default;
	Material&	operator=(Material const&) = delete;
	Material&	operator=(Material&&) = delete;
	bool	Initialize();
	void	Uninitialize();
	bool	SentToGPU(uint32_t const pIndexCount, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjectionMatrix);
private:
	bool	SetMatrixParameters(XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjMatrix);
	void	OutputShaderErrorMessage(ID3D10Blob* const pErrorMsg, WCHAR const* const pShaderFilename);
	bool	CompileShader(WCHAR const* const pShaderFilename, char const* const pEntryPoint, char const* const pShaderTarget, ID3DBlob** pShaderBuffer);
	bool	InitializeVertexInputData(ID3DBlob* const pVertexShader);
	bool	InitializeMatrixBuffer();
private:
	ID3D11VertexShader*	__vertexShader;
	ID3D11PixelShader*	__pixelShader;
	ID3D11InputLayout*	__inputLayout;
	ID3D11Buffer*		__matrixBuffer;
};

#endif