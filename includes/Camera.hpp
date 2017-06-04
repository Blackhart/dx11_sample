#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <DirectXMath.h>

using namespace DirectX;

#define DEG_TO_RAD 0.0174532925f;

class Camera
{
public:
	Camera();
	Camera(Camera const&) = delete;
	Camera(Camera&&) = delete;
	~Camera() = default;
	Camera&	operator=(Camera const&) = delete;
	Camera&	operator=(Camera&&) = delete;
	void		SetPosition(float const x, float const y, float const z);
	void		SetRotation(float const x, float const y, float const z);
	XMFLOAT3	GetPosition() const;
	XMFLOAT3	GetRotation() const;
	void		Render();
	void		GetViewMatrix(XMMATRIX& pViewMatrix) const;
private:
	XMFLOAT3	__position;
	XMFLOAT3	__rotation;
	XMMATRIX	__viewMatrix;
};

#endif