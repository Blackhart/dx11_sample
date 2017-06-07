#include "../includes/Spatial.hpp"

Spatial::Spatial() :
	__geometry{ nullptr },
	__shader { nullptr }
{
	__modelTransform = XMMatrixIdentity();
}

Spatial::Spatial(Geometry* const pGeometry, Shader* const pShader) :
	__geometry{ pGeometry },
	__shader { pShader }
{
	__modelTransform = XMMatrixIdentity();
}

void	Spatial::SetGeometry(Geometry* const pGeometry)
{
	__geometry = pGeometry;
}

void	Spatial::SetShader(Shader* const pShader)
{
	__shader = pShader;
}

Geometry* const	Spatial::GetGeometry() const
{
	return __geometry;
}

Shader* const	Spatial::GetShader() const
{
	return __shader;
}

Geometry const* const	Spatial::GetConstGeometry() const
{
	return __geometry;
}

Shader const* const	Spatial::GetConstShader() const
{
	return __shader;
}