#include "../includes/Camera.hpp"

Camera::Camera()
{
	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	__viewMatrix = XMMatrixIdentity();
}

void	Camera::SetPosition(float const x, float const y, float const z)
{
	__position.x = x;
	__position.y = y;
	__position.z = z;
}

void	Camera::SetRotation(float const x, float const y, float const z)
{
	__rotation.x = x;
	__rotation.y = y;
	__rotation.z = z;
}

XMFLOAT3	Camera::GetPosition() const
{
	return __position;
}

XMFLOAT3	Camera::GetRotation() const
{
	return __rotation;
}

void	Camera::Render()
{
	XMVECTOR	lUpSIMDVector;
	XMVECTOR	lPositionSIMDVector;
	XMVECTOR	lLookAtSIMDVector;
	XMMATRIX	lRotationMatrix;
	XMFLOAT3	lUp;
	XMFLOAT3	lLookAt;
	float		lYaw;
	float		lPitch;
	float		lRoll;

	lUp.x = 0.0f; lUp.y = 1.0f; lUp.z = 0.0f;
	lUpSIMDVector = XMLoadFloat3(&lUp);

	lPositionSIMDVector = XMLoadFloat3(&__position);

	lLookAt.x = 0.0f; lLookAt.y = 0.0f; lLookAt.z = 1.0f;
	lLookAtSIMDVector = XMLoadFloat3(&lLookAt);

	lYaw = __rotation.x * DEG_TO_RAD;
	lPitch = __rotation.y * DEG_TO_RAD;
	lRoll = __rotation.z * DEG_TO_RAD;
	lRotationMatrix = XMMatrixRotationRollPitchYaw(lPitch, lYaw, lRoll);

	lLookAtSIMDVector = XMVector3TransformCoord(lLookAtSIMDVector, lRotationMatrix);
	lUpSIMDVector = XMVector3TransformCoord(lUpSIMDVector, lRotationMatrix);

	lLookAtSIMDVector = XMVectorAdd(lPositionSIMDVector, lLookAtSIMDVector);

	__viewMatrix = XMMatrixLookAtLH(lPositionSIMDVector, lLookAtSIMDVector, lUpSIMDVector);
}

void	Camera::GetViewMatrix(XMMATRIX& pViewMatrix) const
{
	pViewMatrix = __viewMatrix;
}