#include "../includes/Shader.hpp"
#include "../includes/D3DWrapper.hpp"

Shader::Shader() :
	__vertexShader{ nullptr },
	__pixelShader{ nullptr },
	__inputLayout{ nullptr },
	__matrixBuffer{ nullptr }
{

}

bool	Shader::Initialize(ID3D11Device* const pDevice, HWND const pHWND)
{
	HRESULT		lResult;
	ID3DBlob*	lVertexShader = nullptr;
	ID3DBlob*	lPixelShader = nullptr;

	// Compile Vertex + Pixel shaders
	if (!CompileShader(L"VertexShader.hlsl", "vert", "vs_5_0", &lVertexShader, pHWND))
		return false;
	if (!CompileShader(L"PixelShader.hlsl", "frag", "ps_5_0", &lPixelShader, pHWND))
	{
		lVertexShader->Release();
		return false;
	}

	// Create Vertex + Pixel object
	lResult = pDevice->CreateVertexShader(lVertexShader->GetBufferPointer(), lVertexShader->GetBufferSize(), nullptr, &__vertexShader);
	if (FAILED(lResult))
	{
		lVertexShader->Release(); 
		lPixelShader->Release(); 
		return false;
	}
	lResult = pDevice->CreatePixelShader(lPixelShader->GetBufferPointer(), lPixelShader->GetBufferSize(), nullptr, &__pixelShader);
	if (FAILED(lResult))
	{
		lVertexShader->Release();
		lPixelShader->Release();
		return false;
	}

	// Add info about data being sent to the shader
	if (!InitializeVertexInputData(pDevice, lVertexShader))
	{
		lVertexShader->Release();
		lPixelShader->Release();
		return false;
	}

	lVertexShader->Release();
	lPixelShader->Release();

	if (!InitializeMatrixBuffer(pDevice))
		return false;

	return true;
}

bool	Shader::CompileShader(WCHAR const* const pShaderFilename, char const* const pEntryPoint, char const* const pShaderTarget, ID3DBlob** pShaderBuffer, HWND const pHWND)
{
	HRESULT		lResult;
	ID3DBlob*	lErrorMsg = nullptr;

	lResult = D3DCompileFromFile(pShaderFilename, nullptr, nullptr, pEntryPoint, pShaderTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, pShaderBuffer, &lErrorMsg);
	if (FAILED(lResult))
	{
		if (lErrorMsg)
			OutputShaderErrorMessage(lErrorMsg, pHWND, pShaderFilename);
		else
			MessageBox(pHWND, pShaderFilename, L"Missing Shader File", MB_OK);
		return false;
	}
	return true;
}

bool	Shader::InitializeVertexInputData(ID3D11Device* const pDevice, ID3DBlob* const pVertexShader)
{
	HRESULT						lResult;
	D3D11_INPUT_ELEMENT_DESC	lPolygonLayout[2];
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

	lNumElems = sizeof(lPolygonLayout) / sizeof(lPolygonLayout[0]);

	lResult = pDevice->CreateInputLayout(lPolygonLayout, lNumElems, pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize(), &__inputLayout);
	if (FAILED(lResult))
		return false;

	return true;
}

bool	Shader::InitializeMatrixBuffer(ID3D11Device* const pDevice)
{
	return CreateBuffer(pDevice, &__matrixBuffer, sizeof(MatrixBuffer), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, nullptr, 0, 0);
}

void	Shader::Uninitialize()
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

void	Shader::OutputShaderErrorMessage(ID3D10Blob* const pErrorMsg, HWND const pHWND, WCHAR const* const pShaderFilename)
{
	char*			lErrors = nullptr;
	std::ofstream	lStreamOut;

	lErrors = (char*)(pErrorMsg->GetBufferPointer());
	lStreamOut.open("Log.txt");
	lStreamOut << lErrors;
	lStreamOut.close();
	pErrorMsg->Release();
	MessageBox(pHWND, L"Error compiling shader. Take a look to Log.txt!", pShaderFilename, MB_OK);
}

bool	Shader::Render(ID3D11DeviceContext* const pDeviceContext, int const pIndexCount, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjectionMatrix)
{
	if (!SetShaderParameters(pDeviceContext, pWorldMatrix, pViewMatrix, pProjectionMatrix))
		return false;

	pDeviceContext->IASetInputLayout(__inputLayout);

	pDeviceContext->VSSetShader(__vertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(__pixelShader, nullptr, 0);

	pDeviceContext->DrawIndexed(pIndexCount, 0, 0);

	return true;
}

bool	Shader::SetShaderParameters(ID3D11DeviceContext* const pDeviceContext, XMMATRIX const pWorldMatrix, XMMATRIX const pViewMatrix, XMMATRIX const pProjMatrix)
{
	HRESULT						lResult;
	D3D11_MAPPED_SUBRESOURCE	lMappedResource;
	MatrixBuffer*				lMatrixBuffer = nullptr;

	lResult = pDeviceContext->Map(__matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lMappedResource);
	if (FAILED(lResult))
		return false;

	lMatrixBuffer = (MatrixBuffer*)lMappedResource.pData;
	lMatrixBuffer->world = XMMatrixTranspose(pWorldMatrix);
	lMatrixBuffer->view = XMMatrixTranspose(pViewMatrix);
	lMatrixBuffer->projection = XMMatrixTranspose(pProjMatrix);

	pDeviceContext->Unmap(__matrixBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(0, 1, &__matrixBuffer);

	return true;
}