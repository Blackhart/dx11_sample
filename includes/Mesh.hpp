#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdint>
#include <new>

using namespace DirectX;

class Mesh
{
private:
	typedef struct sVertexData
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT3 normal;
	} VertexData;

public:
	Mesh();
	Mesh(Mesh const&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh() = default;
	Mesh&						operator=(Mesh const&) = delete;
	Mesh&						operator=(Mesh&&) = delete;
	bool						Initialize(ID3D11Device* const pDevice);
	void						Uninitialize();
	void						SentToGPU(ID3D11DeviceContext* const pDeviceContext);
	uint32_t					GetIndexCount() const;
	uint32_t					GetVertexCount() const;
	ID3D11Buffer const* const	GetConstVertexBuffer() const;
	ID3D11Buffer const* const	GetConstIndexBuffer() const;
	ID3D11Buffer* const			GetVertexBuffer() const;
	ID3D11Buffer* const			GetIndexBuffer() const;
	uint32_t					GetBufferSize() const;
private:
	bool	InitializeVertexData(VertexData** pVertices, uint32_t** pIndices);
	bool	InitializeBuffers(ID3D11Device* const pDevice, VertexData const* pVertices, uint32_t const* pIndices);
private:
	ID3D11Buffer*	__vertexBuffer;
	ID3D11Buffer*	__indexBuffer;
	uint32_t		__vertexCount;
	uint32_t		__indexCount;
};

#endif