#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera
{
public:
	Camera();
	~Camera();

	vec GetPos();
	void SetPos(vec newPos);

	float GetSpeed();
	void SetSpeed(float newSpeed);

	float* getViewMatrix();
	float* getProjectionMatrix();

public:

	Frustum frustum;

	float nearPlane = 0.3f;
	float farPlane = 1000.f;
	float aspectRatio = 0.0f;
	float vFOV = 0.0f;
	float speed = 0.016f;

	float4x4 mainTransform = float4x4::identity;
	float3 looking;
};

#endif //!__CAMERA_H__