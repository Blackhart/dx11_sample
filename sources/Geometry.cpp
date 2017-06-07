#include "../includes/Geometry.hpp"
#include "../includes/D3DWrapper.hpp"
#include "../includes/tiny_obj_loader.h"

Geometry::Geometry() :
	__vertexBuffer{ nullptr },
	__indexBuffer{ nullptr },
	__vertexCount{ 0 },
	__indexCount{ 0 }
{

}

bool	Geometry::Initialize(ID3D11Device* const pDevice)
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

bool	Geometry::InitializeVertexData(VertexData** pVertices, uint32_t** pIndices)
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

bool	Geometry::InitializeBuffers(ID3D11Device* const pDevice, VertexData const* pVertices, uint32_t const* pIndices)
{
	if (!CreateBuffer(pDevice, &__vertexBuffer, sizeof(VertexData) * __vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0, pVertices, 0, 0))
		return false;

	if (!CreateBuffer(pDevice, &__indexBuffer, sizeof(uint32_t) * __indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0, pIndices, 0, 0))
		return false;

	return true;
}

void	Geometry::Uninitialize()
{
	if (__vertexBuffer)
		__vertexBuffer->Release();

	if (__indexBuffer)
		__indexBuffer->Release();
}

uint32_t	Geometry::GetIndexCount() const
{
	return __indexCount;
}

uint32_t	Geometry::GetVertexCount() const
{
	return __vertexCount;
}

ID3D11Buffer const* const	Geometry::GetConstVertexBuffer() const
{
	return __vertexBuffer;
}

ID3D11Buffer const* const	Geometry::GetConstIndexBuffer() const
{
	return __indexBuffer;
}

ID3D11Buffer* const	Geometry::GetVertexBuffer() const
{
	return __vertexBuffer;
}

ID3D11Buffer* const	Geometry::GetIndexBuffer() const
{
	return __indexBuffer;
}

uint32_t	Geometry::GetBufferSize() const
{
	return sizeof(VertexData);
}