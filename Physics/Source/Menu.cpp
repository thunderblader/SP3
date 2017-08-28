#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Menu(double dt) {
	switch (menustate)
	{
	case MENU:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU5;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
			menustate = LEVEL1;
		break;
	case MENU2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU3;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU;
		break;
	case MENU3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU4;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU2;
		break;
	case MENU4:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU3;
		break;
	case MENU5:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU4;
		break;
	case LEVEL1:
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
	case LEVEL2:
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
	case LEVEL3:
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
	case LEVEL4:
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
	case LEVEL5:
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
	case LEVELBACK:
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
		case MENU:
			RenderMesh(meshList[GEO_MENU], false);
			break;
		case MENU2:
			RenderMesh(meshList[GEO_MENU2], false);
			break;
		case MENU3:
			RenderMesh(meshList[GEO_MENU3], false);
			break;
		case MENU4:
			RenderMesh(meshList[GEO_MENU4], false);
			break;
		case MENU5:
			RenderMesh(meshList[GEO_MENU5], false);
			break;

		case LEVEL1:
			RenderMesh(meshList[GEO_LEVEL1], false);
			break;
		case LEVEL2:
			RenderMesh(meshList[GEO_LEVEL2], false);
			break;
		case LEVEL3:
			RenderMesh(meshList[GEO_LEVEL3], false);
			break;
		case LEVEL4:
			RenderMesh(meshList[GEO_LEVEL4], false);
			break;
		case LEVEL5:
			RenderMesh(meshList[GEO_LEVEL5], false);
			break;
		case LEVELBACK:
			RenderMesh(meshList[GEO_LEVELBACK], false);
			break;

		case LS:
			RenderMesh(meshList[GEO_LS], false);
			break;
		case LS2:
			RenderMesh(meshList[GEO_LS2], false);
			break;
		case LS3:
			RenderMesh(meshList[GEO_LS2], false);
			break;

		case OPTIONS:
			RenderMesh(meshList[GEO_OPTIONS], false);
			break;
		case OPTIONS2:
			RenderMesh(meshList[GEO_OPTIONS2], false);
			break;

		case INSTRUCTIONS:
			RenderMesh(meshList[GEO_INSTRUCTIONS], false);
			break;
		}
		modelStack.PopMatrix();
	}
}