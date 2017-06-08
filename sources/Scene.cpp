#include "../includes/Scene.hpp"

void	Scene::AddObject(SceneObject* const pMesh)
{
	__objects.push_back(pMesh);
}

SceneObject* const	Scene::GetObjectAt(uint32_t pIndex) const
{
	if (pIndex < GetObjectCount())
		throw new std::out_of_range("Scene::GetObjectAt() : Invalid Range Exception!");
	return __objects[pIndex];
}

uint32_t	Scene::GetObjectCount() const
{
	return __objects.size();
}