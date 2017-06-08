#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>
#include <cstdint>
#include <stdexcept>

#include "SceneObject.hpp"

class Scene
{
public:
	Scene() = default;
	Scene(Scene const&) = delete;
	Scene(Scene&&) = delete;
	~Scene() = default;
	Scene&				operator=(Scene const&) = delete;
	Scene&				operator=(Scene&&) = delete;
	void				AddObject(SceneObject* const pMesh);
	SceneObject* const	GetObjectAt(uint32_t pIndex) const;
	uint32_t			GetObjectCount() const;
private:
	std::vector<SceneObject*>	__objects;
};

#endif