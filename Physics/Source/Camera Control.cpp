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
	if (free_look == true)
	{
		if (KeyboardController::GetInstance()->IsKeyDown('A'))
		{
			camera.position.x -= (float)dt * 50.f;
			camera.target.x -= (float)dt * 50.f;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('D'))
		{
			camera.position.x += (float)dt * 50.f;
			camera.target.x += (float)dt * 50.f;
		}
	}
	else
	{
		camera.position.x = m_player->GetPlayerPos().x - 30;
		camera.target.x = m_player->GetPlayerPos().x - 30;
		if (camera.position.x < -1000)
		{
			camera.position.x = -1000;
			camera.target.x = -1000;
		}
	}
}