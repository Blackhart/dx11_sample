#include "../includes/SceneObject.hpp"

SceneObject::SceneObject(Mesh* const pMesh, Material* const pMaterial) :
	__mesh{ pMesh },
	__material { pMaterial }
{
	__modelTransform = XMMatrixIdentity();
}

void	SceneObject::SetMesh(Mesh& pMesh)
{
	__mesh = &pMesh;
}

void	SceneObject::SetMaterial(Material& pMaterial)
{
	__material = &pMaterial;
}

Mesh* const	SceneObject::GetMesh() const
{
	return __mesh;
}

Material* const	SceneObject::GetMaterial() const
{
	return __material;
}

Mesh const*	const	SceneObject::GetConstMesh() const
{
	return __mesh;
}

Material const* const	SceneObject::GetConstMaterial() const
{
	return __material;
}