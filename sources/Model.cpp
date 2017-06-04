#include "../includes/Model.hpp"
#include "../includes/D3DWrapper.hpp"

Model::Model() :
	__vertexBuffer{ nullptr },
	__indexBuffer{ nullptr },
	__vertexCount{ 0 },
	__indexCount{ 0 }
{

}

bool	Model::Initialize(ID3D11Device* const pDevice)
{
	VertexData*	lVertices = nullptr;
	uint32_t*	lIndices = nullptr;

	if (!InitializeVertexData(&lVertices, &lIndices))
		return false;

	if (!InitializeBuffers(pDevice, lVertices, lIndices))
	{
		delete[] lVertices;
		delete[] lIndices;
		return false;
	}

	delete[] lVertices;
	delete[] lIndices;

	return true;
}

bool	Model::InitializeVertexData(VertexData** pVertices, uint32_t** pIndices)
{
	__vertexCount = 4;
	__indexCount = 6;

	*pVertices = new (std::nothrow) VertexData[__vertexCount];
	if (*pVertices == nullptr)
		return false;

	*pIndices = new (std::nothrow) uint32_t[__indexCount];
	if (*pIndices == nullptr)
	{
		delete[] *pVertices;
		return false;
	}

	(*pVertices)[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	(*pVertices)[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	(*pVertices)[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);
	(*pVertices)[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	(*pVertices)[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	(*pVertices)[2].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	(*pVertices)[3].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	(*pVertices)[3].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	(*pIndices)[0] = 0;
	(*pIndices)[1] = 1;
	(*pIndices)[2] = 2;
	(*pIndices)[3] = 0;
	(*pIndices)[4] = 2;
	(*pIndices)[5] = 3;

	return true;
}

bool	Model::InitializeBuffers(ID3D11Device* const pDevice, VertexData const* pVertices, uint32_t const* pIndices)
{
	if (!CreateBuffer(pDevice, &__vertexBuffer, sizeof(VertexData) * __vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0, pVertices, 0, 0))
		return false;

	if (!CreateBuffer(pDevice, &__indexBuffer, sizeof(uint32_t) * __indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0, pIndices, 0, 0))
		return false;

	return true;
}

void	Model::Uninitialize()
{
	if (__vertexBuffer)
		__vertexBuffer->Release();

	if (__indexBuffer)
		__indexBuffer->Release();
}

void	Model::Render(ID3D11DeviceContext* const pDeviceContext)
{
	UINT const	lStride = (UINT)sizeof(VertexData);
	UINT const	lOffset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &__vertexBuffer, &lStride, &lOffset);
	pDeviceContext->IASetIndexBuffer(__indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

uint8_t	Model::GetIndexCount() const
{
	return __indexCount;
}