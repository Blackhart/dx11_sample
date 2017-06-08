#ifndef __SCENE_OBJECT_HPP__
#define __SCENE_OBJECT_HPP__

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Mesh;
class Material;

class SceneObject
{
public:
	SceneObject(Mesh* const pMesh, Material* const pMaterial);
	SceneObject(SceneObject const&) = delete;
	SceneObject(SceneObject&&) = delete;
	~SceneObject() = default;
	SceneObject&			operator=(SceneObject const&) = delete;
	SceneObject&			operator=(SceneObject&&) = delete;
	void					SetMesh(Mesh& pMesh);
	void					SetMaterial(Material& pMaterial);
	Mesh* const				GetMesh() const;
	Material* const			GetMaterial() const;
	Mesh const*	const		GetConstMesh() const;
	Material const* const	GetConstMaterial() const;
private:
	Mesh*		__mesh;
	Material*	__material;
	XMMATRIX	__modelTransform;
};

#endif