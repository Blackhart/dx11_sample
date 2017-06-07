#include "../includes/Mesh.hpp"

Mesh::Mesh(Geometry* const pGeometry, Shader* const pShader) :
	__geometry{ pGeometry },
	__shader { pShader }
{
	__modelTransform = XMMatrixIdentity();
}

void	Mesh::SetGeometry(Geometry& pGeometry)
{
	__geometry = &pGeometry;
}

void	Mesh::SetShader(Shader& pShader)
{
	__shader = &pShader;
}

Geometry* const	Mesh::GetGeometry() const
{
	return __geometry;
}

Shader* const	Mesh::GetShader() const
{
	return __shader;
}

Geometry const*	const	Mesh::GetConstGeometry() const
{
	return __geometry;
}

Shader const* const	Mesh::GetConstShader() const
{
	return __shader;
}