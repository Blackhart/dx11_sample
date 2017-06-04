#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdint>
#include <new>

using namespace DirectX;

class Model
{
private:
	typedef struct sVertexData
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	} VertexData;

public:
	Model() = default;
	Model(Model const&) = delete;
	Model(Model&&) = delete;
	~Model() = default;
	Model&	operator=(Model const&) = delete;
	Model&	operator=(Model&&) = delete;
	bool	Initialize(ID3D11Device* const pDevice);
	void	Uninitialize();
	void	Render(ID3D11DeviceContext* const pDeviceContext);
	uint8_t	GetIndexCount() const;
private:
	bool	InitializeVertexData(VertexData** pVertices, uint8_t** pIndices);
	bool	InitializeBuffers(ID3D11Device* const pDevice, VertexData const* pVertices, uint8_t const* pIndices);
private:
	ID3D11Buffer*	__vertexBuffer = nullptr;
	ID3D11Buffer*	__indexBuffer = nullptr;
	uint8_t		__vertexCount = 0;
	uint8_t		__indexCount = 0;
};

#endif