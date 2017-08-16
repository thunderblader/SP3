#include "Scene01.h"
#include "Application.h"

void Scene01::Camera_Control(double dt)
{
	if (Application::IsKeyPressed('O'))
	{
		if (free_look == false)
		{
			free_look = true;
		}
	}
	if (Application::IsKeyPressed('P'))
	{
		if (free_look == true)
		{
			free_look = false;
		}
	}
	if (free_look == false)
	{
		camera.position.x = 0;
		camera.target.x = 0;
		camera.position.y = 0;
		camera.target.y = 0;
	}

	if (free_look == true)
	{
		//camera.position.x = test->pos.x - 30;
		//camera.target.x = test->pos.x - 30;
		//camera.position.y = test->pos.y - 45;
		//camera.target.y = test->pos.y - 45;

		if (Application::IsKeyPressed('A'))
		{
			//CSoundEngine::GetInstance()->PlayASound("Jump");
			camera.position.x -= dt * (float)(50);
			camera.target.x -= dt * (float)(50);

		}
		else if (Application::IsKeyPressed('D'))
		{
			camera.position.x += dt * (float)(50);
			camera.target.x += dt * (float)(50);
		}
	}
	std::cout << camera.position.x << std::endl;
	//std::cout << free_look << std::endl;
}