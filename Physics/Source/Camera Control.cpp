#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Camera_Control(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
	{
		if (free_look == false)
		{
			free_look = true;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
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

		if (KeyboardController::GetInstance()->IsKeyDown('A'))
		{
			//CSoundEngine::GetInstance()->PlayASound("Jump");
			camera.position.x -= dt * (float)(50);
			camera.target.x -= dt * (float)(50);

		}
		else if (KeyboardController::GetInstance()->IsKeyDown('D'))
		{
			camera.position.x += dt * (float)(50);
			camera.target.x += dt * (float)(50);
		}
	}
	//std::cout << camera.position.x << std::endl;
	//std::cout << free_look << std::endl;
}