#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "Camera.h"

ModuleCamera::ModuleCamera(string _name, bool _active):Module(_name,_active)
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	bool ret = true;

	mainCamera = new Camera();

	return ret;
}

bool ModuleCamera::Update()
{
	bool ret = true;

	float3 camPos = mainCamera->GetPos();
	float speed = mainCamera->GetSpeed();
	float3 camFront = mainCamera->frustum.Front();
	float3 camUp = mainCamera->frustum.Up();
	float3 camRight = mainCamera->frustum.WorldRight();

	//X Axis transform
	if (myApp->m_input->GetKey(SDL_SCANCODE_A) == REPEAT)
	{
		camPos -= camRight.Normalized()*speed;
	}
	if (myApp->m_input->GetKey(SDL_SCANCODE_D) == REPEAT)
	{
		camPos += camRight.Normalized()*speed;
	}
	
	//Y Axis transform
	if (myApp->m_input->GetKey(SDL_SCANCODE_Q) == REPEAT)
	{
		camPos -= float3::unitY*speed;
	}
	if (myApp->m_input->GetKey(SDL_SCANCODE_E) == REPEAT)
	{
		camPos += float3::unitY*speed;
	}

	//Z Axis transform
	if (myApp->m_input->GetKey(SDL_SCANCODE_S) == REPEAT)
	{
		camPos -= camFront*speed;
	}
	if (myApp->m_input->GetKey(SDL_SCANCODE_W) == REPEAT)
	{
		camPos += camFront *speed;
	}

	if (myApp->m_input->GetMouseButton(SDL_BUTTON_LEFT)==REPEAT)
	{
		if (myApp->m_input->motionX != 0)
		{
			Quat rotation = Quat::RotateAxisAngle(camUp,myApp->m_input->motionX*0.016f);
			camFront = rotation.Mul(camFront).Normalized();
			//camUp = rotation.Mul(camUp).Normalized();
		}

		if (myApp->m_input->motionY != 0)
		{
			Quat rotation = Quat::RotateAxisAngle(mainCamera->frustum.WorldRight(),myApp->m_input->motionY*0.016f);

			if (rotation.Mul(mainCamera->frustum.Up()).Normalized().y > 0.0f)
			{
				camUp = rotation.Mul(camUp).Normalized();
				//camFront = rotation.Mul(camFront).Normalized();
			}
		}
	}

	if (myApp->m_input->GetMouseButton(SDL_BUTTON_RIGHT) == DOWN)
	{
		LOG("MOUSEEEEEEEE RIIIIIGGGHHHHTTTTTT!!!!!!!!!!!!!");
	}

	mainCamera->SetPos(camPos);
	mainCamera->frustum.SetFront(camFront);
	mainCamera->frustum.SetUp(camUp);

	return ret;
}

bool ModuleCamera::CleanUp()
{
	bool ret = true;

	if (mainCamera)
	{
		delete mainCamera;
		mainCamera = nullptr;
	}

	return ret;
}
