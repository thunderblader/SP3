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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = LEVEL1;
		break;
	case MENU2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU3;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = LS;
		break;
	case MENU3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU4;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU2;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = INSTRUCTIONS;
		break;
	case MENU4:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = OPTIONS;
		break;
	case MENU5:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = MENU;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = MENU4;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			exit(0);
		break;


	case LEVEL1:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
			menustate = LEVEL5;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LEVEL2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LEVELBACK;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
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
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			display = false;
			newlevel = 5;
		}
		break;
	case LEVELBACK:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = LEVEL1;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = MENU;
		break;

	case LS:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LS2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LS3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			file.Load_Data(item_node->root);
			Gold = file.get_gold();
			Score = file.get_score();
			shop.set_gold(Gold);
		}
		break;
	case LS2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = LS;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LS3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			file.Save_Data(Level_data, Score, Gold, item_node->root);
		}
		break;
	case LS3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = LS;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = MENU;
		break;


	case INSTRUCTIONS:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = MENU;
		break;

	case OPTIONS:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = OPTIONS2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = OPTIONS3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			sound_engine->setSoundVolume(0);
		}
		break;
	case OPTIONS2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = OPTIONS;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = OPTIONS3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			sound_engine->setSoundVolume(100);
		}
		break;
	case OPTIONS3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = OPTIONS;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = MENU;
		break;

	case WIN1:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = WIN2;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = WIN3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			Reset(newlevel);
		}
		break;
	case WIN2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT) || KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
			menustate = WIN1;
		else if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = WIN3;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			newlevel++;
		break;
	case WIN3:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
			menustate = WIN1;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
			menustate = MENU;
		break;

	case LOSE1:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LOSE2;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			Reset(newlevel);
		}
		break;
	case LOSE2:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
			menustate = LOSE1;
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			// reset
		}
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
			RenderMesh(meshList[GEO_LS3], false);
			break;

		case OPTIONS:
			RenderMesh(meshList[GEO_OPTIONS], false);
			break;
		case OPTIONS2:
			RenderMesh(meshList[GEO_OPTIONS2], false);
			break;
		case OPTIONS3:
			RenderMesh(meshList[GEO_OPTIONS3], false);
			break;

		case INSTRUCTIONS:
			RenderMesh(meshList[GEO_INSTRUCTIONS], false);
			break;

		case WIN1:
			RenderMesh(meshList[GEO_WIN1], false);
			break;
		case WIN2:
			RenderMesh(meshList[GEO_WIN2], false);
			break;
		case WIN3:
			RenderMesh(meshList[GEO_WIN3], false);
			break;

		case LOSE1:
			RenderMesh(meshList[GEO_LOSE1], false);
			break;
		case LOSE2:
			RenderMesh(meshList[GEO_LOSE2], false);
			break;
		}
		modelStack.PopMatrix();
	}
}