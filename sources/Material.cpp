#include "../includes/Material.hpp"
#include "../includes/D3DFunctionalities.hpp"
#include "../includes/Window.hpp"

Material::Material() :
	__vertexShader{ nullptr },
	__pixelShader{ nullptr },
	__inputLayout{ nullptr },
	__matrixBuffer{ nullptr }
{

}

bool	Material::Initialize()
{
	HRESULT		lResult;
	ID3DBlob*	lVertexShader = nullptr;
	ID3DBlob*	lPixelShader = nullptr;

	// Compile Vertex + Pixel shaders
	if (!CompileShader(L"VertexShader.hlsl", "vert", "vs_5_0", &lVertexShader))
		return false;
	if (!CompileShader(L"PixelShader.hlsl", "frag", "ps_5_0", &lPixelShader))
	{
		lVertexShader->Release();
		return false;
	}

	// Create Vertex + Pixel object
	lResult = D3DInst->GetDevice()->CreateVertexShader(lVertexShader->GetBufferPointer(), lVertexShader->GetBufferSize(), nullptr, &__vertexShader);
	if (FAILED(lResult))
	{
		lVertexShader->Release(); 
		lPixelShader->Release(); 
		return false;
	}
	lResult = D3DInst->GetDevice()->CreatePixelShader(lPixelShader->GetBufferPointer(), lPixelShader->GetBufferSize(), nullptr, &__pixelShader);
	if (FAILED(lResult))
	{
		lVertexShader->Release();
		lPixelShader->Release();
		return false;
	}

	// Add info about data being sent to the shader
	if (!InitializeVertexInputData(lVertexShader))
	{
		lVertexShader->Release();
		lPixelShader->Release();
		return false;
	}

	lVertexShader->Release();
	lPixelShader->Release();

	if (!InitializeMatrixBuffer())
		return false;

	return true;
}

bool	Material::CompileShader(WCHAR const* const pShaderFilename, char const* const pEntryPoint, char const* const pShaderTarget, ID3DBlob** pShaderBuffer)
{
	HRESULT		lResult;
	ID3DBlob*	lErrorMsg = nullptr;

	lResult = D3DCompileFromFile(pShaderFilename, nullptr, nullptr, pEntryPoint, pShaderTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, pShaderBuffer, &lErrorMsg);
	if (FAILED(lResult))
	{
		if (lErrorMsg)
			OutputShaderErrorMessage(lErrorMsg, pShaderFilename);
		else
			MessageBox(WindowInst->GetWindow(), pShaderFilename, L"Missing Shader File", MB_OK);
		return false;
	}
	return true;
}

bool	Material::InitializeVertexInputData(ID3DBlob* const pVertexShader)
{
	HRESULT						lResult;
	D3D11_INPUT_ELEMENT_DESC	lPolygonLayout[3];
	uint32_t					lNumElems = 0;

	lPolygonLayout[0].SemanticName = "POSITION";
	lPolygonLayout[0].SemanticIndex = 0;
	lPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	lPolygonLayout[0].InputSlot = 0;
	lPolygonLayout[0].AlignedByteOffset = 0;
	lPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	lPolygonLayout[0].InstanceDataStepRate = 0;

	lPolygonLayout[1].SemanticName = "COLOR";
	lPolygonLayout[1].SemanticIndex = 0;
	lPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	lPolygonLayout[1].InputSlot = 0;
	lPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	lPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	lPolygonLayout[1].InstanceDataStepRate = 0;

	lPolygonLayout[2].SemanticName = "NORMAL";
	lPolygonLayout[2].SemanticIndex = 0;
	lPolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	lPolygonLayout[2].InputSlot = 0;
	lPolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	lPolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	lPolygonLayout[2].InstanceDataStepRate = 0;

	lNumElems = sizeof(lPolygonLayout) / sizeof(lPolygonLayout[0]);

	lResult = D3DInst->GetDevice()->CreateInputLayout(lPolygonLayout, lNumElems, pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize(), &__inputLayout);
	if (FAILED(lResult))
		return false;

	return true;
}

bool	Material::InitializeMatrixBuffer()
{
	return CreateBuffer(&__matrixBuffer, sizeof(MatrixBuffer), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, nullptr, 0, 0);
}

void	Material::Uninitialize()
{
	if (__matrixBuffer)
		__matrixBuffer->Release();

	if (__inputLayout)
		__inputLayout->Release();

	if (__pixelShader)
		__pixelShader->Release();

	if (__vertexShader)
		__vertexShader->Release();
}

void	Material::OutputShaderErrorMessage(ID3D10Blob* const pErrorMsg, WCHAR const* const pShaderFilename)
{
	char*			lErrors = nullptr;
	std::ofstream	lStreamOut;

	lErrors = (char*)(pErrorMsg->GetBufferPointer());
	lStreamOut.open("Log.txt");
	lStreamOut << lErrors;
	lStreamOut.close();
	pErrorMsg->Release();
	MessageBox(WindowInst->GetWindow(), L"Error compiling shader. Take a look to Log.txt!", pShaderFilename, MB_OK);
}

bool	Material::SentToGPU(uint32_t const pIndexCount, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjectionMatrix)
{
	if (!SetMatrixParameters(pWorldMatrix, pViewMatrix, pProjectionMatrix))
		return false;

	D3DInst->GetDeviceContext()->IASetInputLayout(__inputLayout);

	D3DInst->GetDeviceContext()->VSSetShader(__vertexShader, nullptr, 0);
	D3DInst->GetDeviceContext()->PSSetShader(__pixelShader, nullptr, 0);

	return true;
}

bool	Material::SetMatrixParameters(XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjMatrix)
{
	HRESULT						lResult;
	D3D11_MAPPED_SUBRESOURCE	lMappedResource;
	MatrixBuffer*				lMatrixBuffer = nullptr;

	lResult = D3DInst->GetDeviceContext()->Map(__matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lMappedResource);
	if (FAILED(lResult))
		return false;

	lMatrixBuffer = (MatrixBuffer*)lMappedResource.pData;
	lMatrixBuffer->world = XMMatrixTranspose(pWorldMatrix);
	lMatrixBuffer->view = XMMatrixTranspose(pViewMatrix);
	lMatrixBuffer->projection = XMMatrixTranspose(pProjMatrix);

	D3DInst->GetDeviceContext()->Unmap(__matrixBuffer, 0);

	D3DInst->GetDeviceContext()->VSSetConstantBuffers(0, 1, &__matrixBuffer);

	return true;
}