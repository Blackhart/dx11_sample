#ifndef __SPATIAL_HPP__
#define __SPATIAL_HPP__

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Geometry;
class Shader;

class Spatial
{
public:
	Spatial();
	Spatial(Geometry* const pGeometry, Shader* const pShader = nullptr);
	Spatial(Spatial const&) = delete;
	Spatial(Spatial&&) = delete;
	~Spatial() = default;
	Spatial&				operator=(Spatial const&) = delete;
	Spatial&				operator=(Spatial&&) = delete;
	void					SetGeometry(Geometry* const pGeometry);
	void					SetShader(Shader* const pShader);
	Geometry* const			GetGeometry() const;
	Shader* const			GetShader() const;
	Geometry const* const	GetConstGeometry() const;
	Shader const* const		GetConstShader() const;
private:
	Geometry*	__geometry;
	Shader*		__shader;
	XMMATRIX	__modelTransform;
};

#endif