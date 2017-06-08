#include "../includes/Mesh.hpp"
#include "../includes/D3DWrapper.hpp"
#include "../includes/tiny_obj_loader.h"

Mesh::Mesh() :
	__vertexBuffer{ nullptr },
	__indexBuffer{ nullptr },
	__vertexCount{ 0 },
	__indexCount{ 0 }
{

}

bool	Mesh::Initialize(ID3D11Device* const pDevice)
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

bool	Mesh::InitializeVertexData(VertexData** pVertices, uint32_t** pIndices)
{
	tinyobj::attrib_t				lAttrib;
	std::vector<tinyobj::shape_t>	lShapes;
	std::string						lError;

	if (!tinyobj::LoadObj(&lAttrib, &lShapes, nullptr, &lError, "cars.obj", nullptr, true))
		return false;

	__vertexCount = lAttrib.vertices.size() / 3;
	for (uint32_t s = 0; s < lShapes.size(); ++s)
		__indexCount += lShapes[s].mesh.indices.size();

	*pVertices = new (std::nothrow) VertexData[__vertexCount];
	if (*pVertices == nullptr)
		return false;

	*pIndices = new (std::nothrow) uint32_t[__indexCount];
	if (*pIndices == nullptr)
	{
		delete[] *pVertices;
		return false;
	}

	uint32_t lArrayElem = 0;

	for (uint32_t i = 0; i < lAttrib.vertices.size(); i += 3, ++lArrayElem)
	{
		(*pVertices)[lArrayElem].position = XMFLOAT3(lAttrib.vertices[i], lAttrib.vertices[i+1], lAttrib.vertices[i+2]);
		(*pVertices)[lArrayElem].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		(*pVertices)[lArrayElem].normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	lArrayElem = 0;

	for (size_t s = 0; s < lShapes.size(); s++)
	{
		for (size_t f = 0; f < lShapes[s].mesh.indices.size(); ++f, ++lArrayElem)
		{
			tinyobj::index_t idx = lShapes[s].mesh.indices[f];
			(*pIndices)[lArrayElem] = idx.vertex_index;
			(*pVertices)[idx.vertex_index].normal.x += lAttrib.normals[3 * idx.normal_index + 0];
			(*pVertices)[idx.vertex_index].normal.y += lAttrib.normals[3 * idx.normal_index + 1];
			(*pVertices)[idx.vertex_index].normal.z += lAttrib.normals[3 * idx.normal_index + 2];
		}
	}

	return true;
}

bool	Mesh::InitializeBuffers(ID3D11Device* const pDevice, VertexData const* pVertices, uint32_t const* pIndices)
{
	if (!CreateBuffer(pDevice, &__vertexBuffer, sizeof(VertexData) * __vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0, pVertices, 0, 0))
		return false;

	if (!CreateBuffer(pDevice, &__indexBuffer, sizeof(uint32_t) * __indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0, pIndices, 0, 0))
		return false;

	return true;
}

void	Mesh::Uninitialize()
{
	if (__vertexBuffer)
		__vertexBuffer->Release();

	if (__indexBuffer)
		__indexBuffer->Release();
}

void	Mesh::SentToGPU(ID3D11DeviceContext* const pDeviceContext)
{
	ID3D11Buffer* const lVertexBuffer = GetVertexBuffer();
	ID3D11Buffer* const lIndexBuffer = GetIndexBuffer();
	UINT lBufferSize = GetBufferSize();
	UINT lOffset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &lVertexBuffer, &lBufferSize, &lOffset);
	pDeviceContext->IASetIndexBuffer(lIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

uint32_t	Mesh::GetIndexCount() const
{
	return __indexCount;
}

uint32_t	Mesh::GetVertexCount() const
{
	return __vertexCount;
}

ID3D11Buffer const* const	Mesh::GetConstVertexBuffer() const
{
	return __vertexBuffer;
}

ID3D11Buffer const* const	Mesh::GetConstIndexBuffer() const
{
	return __indexBuffer;
}

ID3D11Buffer* const	Mesh::GetVertexBuffer() const
{
	return __vertexBuffer;
}

ID3D11Buffer* const	Mesh::GetIndexBuffer() const
{
	return __indexBuffer;
}

uint32_t	Mesh::GetBufferSize() const
{
	return sizeof(VertexData);
}