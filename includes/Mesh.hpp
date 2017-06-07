#ifndef __SPATIAL_HPP__
#define __SPATIAL_HPP__

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Geometry;
class Shader;

class Mesh
{
public:
	Mesh(Geometry* const pGeometry, Shader* const pShader);
	Mesh(Mesh const&) = delete;
	Mesh(Mesh&&) = delete;
	~Mesh() = default;
	Mesh&					operator=(Mesh const&) = delete;
	Mesh&					operator=(Mesh&&) = delete;
	void					SetGeometry(Geometry& pGeometry);
	void					SetShader(Shader& pShader);
	Geometry* const			GetGeometry() const;
	Shader* const			GetShader() const;
	Geometry const*	const	GetConstGeometry() const;
	Shader const* const		GetConstShader() const;
private:
	Geometry*	__geometry;
	Shader*		__shader;
	XMMATRIX	__modelTransform;
};

#endif