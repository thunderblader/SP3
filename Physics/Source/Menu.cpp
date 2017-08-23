#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Menu(double dt) {
	if (menuBounce < 0.25f)
		menuBounce += float(dt);
	if (menuBounce > 0.25f)
	{
		switch (menustate)
		{
		case 1:
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			{
				menustate = MENU2;
				menuBounce = 0;
			}
			else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			{
				menustate = MENU5;
				menuBounce = 0;
			}
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
			{
				display = false;
			}
			break;
		case 2:
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			{
				menustate = MENU3;
				menuBounce = 0;
			}
			else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			{
				menustate = MENU;
				menuBounce = 0;
			}
			break;
		case 3:
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			{
				menustate = MENU4;
				menuBounce = 0;
			}
			else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			{
				menustate = MENU2;
				menuBounce = 0;
			}
			break;
		case 4:
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			{
				menustate = MENU5;
				menuBounce = 0;
			}
			else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			{
				menustate = MENU3;
				menuBounce = 0;
			}
			break;
		case 5:
			if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			{
				menustate = MENU;
				menuBounce = 0;
			}
			else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			{
				menustate = MENU4;
				menuBounce = 0;
			}
			break;
		}
	}
}

void Scene01::RenderMenu() {
	if (display)
	{
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + m_worldWidth / 2, camera.position.y + m_worldHeight / 2, 10);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		switch (menustate)
		{
		case 1:
			RenderMesh(meshList[GEO_MENU], false);
			break;
		case 2:
			RenderMesh(meshList[GEO_MENU2], false);
			break;
		case 3:
			RenderMesh(meshList[GEO_MENU3], false);
			break;
		case 4:
			RenderMesh(meshList[GEO_MENU4], false);
			break;
		case 5:
			RenderMesh(meshList[GEO_MENU5], false);
			break;
		case 6:
			RenderMesh(meshList[GEO_START], false);
			break;
		case 7:
			RenderMesh(meshList[GEO_START2], false);
			break;
		case 8:
			RenderMesh(meshList[GEO_LS], false);
			break;
		case 9:
			RenderMesh(meshList[GEO_LS2], false);
			break;
		case 10:
			RenderMesh(meshList[GEO_LS2], false);
			break;
		case 11:
			RenderMesh(meshList[GEO_OPTIONS], false);
			break;
		case 12:
			RenderMesh(meshList[GEO_OPTIONS], false);
			break;
		case 13:
			RenderMesh(meshList[GEO_INSTRUCTIONS], false);
			break;
		}
		modelStack.PopMatrix();
	}
}