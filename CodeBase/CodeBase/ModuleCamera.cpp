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

	//X Axis transform
	if (myApp->m_input->GetKey(SDL_SCANCODE_A) == REPEAT)
	{
		camPos -= float3::unitX*speed;
	}
	if (myApp->m_input->GetKey(SDL_SCANCODE_D) == REPEAT)
	{
		camPos += float3::unitX*speed;
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
		camPos -= float3::unitZ*speed;
	}
	if (myApp->m_input->GetKey(SDL_SCANCODE_W) == REPEAT)
	{
		camPos += float3::unitZ*speed;
	}

	mainCamera->SetPos(camPos);

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
