#include "Application.h"
#include "Globals.h"
#include "Camera.h"

Camera::Camera()
{
	// For conveniency, allow automatic initialization of the graphics API and handedness in use.
	// If neither of the #defines are set, user must specify per-instance.

	frustum.SetKind(FrustumSpaceGL, FrustumLeftHanded);

	frustum.SetViewPlaneDistances(nearPlane, farPlane);
	frustum.SetPos(float3(0.0f, 0.0f, -5.0f));
	frustum.SetUp(float3::unitY);
	frustum.SetFront(-float3::unitZ);

	//this is the fov that we are goping to calculate
	vFOV = DEGTORAD * 90.0f;
	aspectRatio = 16.0f / 9.0f;

	//The horizontal FOV is automaticalli calculated
	frustum.SetVerticalFovAndAspectRatio(vFOV, aspectRatio);
}


Camera::~Camera()
{
}

vec Camera::GetPos()
{
	return frustum.Pos();
}

void Camera::SetPos(vec newPos)
{
	frustum.SetPos(newPos);
}

float Camera::GetSpeed()
{
	return speed;
}

void Camera::SetSpeed(float newSpeed)
{
	speed = newSpeed;
}

//Returns the View Matrix ready to use with OpenGL
float* Camera::getViewMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}

//Returns the View Matrix ready to use with OpenGL
float * Camera::getProjectionMatrix()
{
	static float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
}