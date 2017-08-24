#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Menu(double dt) {
	switch (menustate)
	{
	case 1:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU5;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
			menustate = LEVEL1;
		break;
	case 2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU3;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU;
		break;
	case 3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU4;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU2;
		break;
	case 4:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU3;
		break;
	case 5:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU4;
		break;
	case 14:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			display = false;
			newlevel = 1;
		}
		break;
	case 15:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL1;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL3;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			display = false;
			newlevel = 2;
		}
		break;
	case 16:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL4;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			display = false;
			newlevel = 3;
		}
		break;
	case 17:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL3;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			display = false;
			newlevel = 4;
		}
		break;
	case 18:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL4;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL1;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
		{
			display = false;
			newlevel = 5;
		}
		break;
	case 19:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = LEVEL1;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
			menustate = MENU;
		break;
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
		case 14:
			RenderMesh(meshList[GEO_LEVEL1], false);
			break;
		case 15:
			RenderMesh(meshList[GEO_LEVEL2], false);
			break;
		case 16:
			RenderMesh(meshList[GEO_LEVEL3], false);
			break;
		case 17:
			RenderMesh(meshList[GEO_LEVEL4], false);
			break;
		case 18:
			RenderMesh(meshList[GEO_LEVEL5], false);
			break;
		case 19:
			RenderMesh(meshList[GEO_LEVELBACK], false);
			break;
		}
		modelStack.PopMatrix();
	}
}