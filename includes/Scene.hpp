#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <vector>

#include "Mesh.hpp"

class Scene
{
public:
	Scene() = default;
	Scene(Scene const&) = delete;
	Scene(Scene&&) = delete;
	~Scene() = default;
	Scene&	operator=(Scene const&) = delete;
	Scene&	operator=(Scene&&) = delete;
private:
	std::vector<Mesh*>	__objects;
};

#endif