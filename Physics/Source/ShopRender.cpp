#include "Scene01.h"

void Scene01::Shop_Render()
{
	string Gold_string = to_string(Gold);
	string Price_string = to_string(item_node->root.price);
	float scaleX, scaleY, posX, posY;
	scaleX = 20.f;
	scaleY = 10.f;

	modelStack.Translate(camera.position.x + 65, camera.position.y + 50, 1);
	modelStack.Scale((float)Application::GetWindowWidth(), 85, 1);
	RenderMesh(meshList[GEO_SCREEN], false);

	RenderMeshIn2D(meshList[GEO_SHOP], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.5f, m_worldHeight - scaleY - scaleY * 0.5f);
	if (item_node->root.id == 1)
	{
		RenderMeshIn2D(meshList[GEO_SIZE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 2.5f));
	}
	else if (item_node->root.id == 2)
	{
		RenderMeshIn2D(meshList[GEO_SPEED], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 2.5f));
	}
	else if (item_node->root.id == 3)
	{
		RenderMeshIn2D(meshList[GEO_JUMP], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 2.5f));
	}
	else if (item_node->root.id == 4)
	{
		RenderMeshIn2D(meshList[GEO_BOOST], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 2.5f));
	}
	RenderMeshIn2D(meshList[GEO_PRICE], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 3.f));
	RenderMeshIn2D(meshList[GEO_GOLD], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 6.f));
	for (unsigned i = 0; i < Gold_string.size(); ++i)
	{
		posX = m_worldWidth * 0.3f + (i * 5.f);
		posY = m_worldHeight - (scaleY * 6.f);

		switch (Gold_string[i])
		{
		case '1':
			RenderMeshIn2D(meshList[GEO_NO_1], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '2':
			RenderMeshIn2D(meshList[GEO_NO_2], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '3':
			RenderMeshIn2D(meshList[GEO_NO_3], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '4':
			RenderMeshIn2D(meshList[GEO_NO_4], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '5':
			RenderMeshIn2D(meshList[GEO_NO_5], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '6':
			RenderMeshIn2D(meshList[GEO_NO_6], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '7':
			RenderMeshIn2D(meshList[GEO_NO_7], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '8':
			RenderMeshIn2D(meshList[GEO_NO_8], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '9':
			RenderMeshIn2D(meshList[GEO_NO_9], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '0':
			RenderMeshIn2D(meshList[GEO_NO_0], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		default: break;
		}
	}
	for (unsigned i = 0; i < Price_string.size(); ++i)
	{
		posX = m_worldWidth * 0.3f + (i * 5.f);
		posY = m_worldHeight - (scaleY * 3.f);

		switch (Price_string[i])
		{
		case '1':
			RenderMeshIn2D(meshList[GEO_NO_1], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '2':
			RenderMeshIn2D(meshList[GEO_NO_2], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '3':
			RenderMeshIn2D(meshList[GEO_NO_3], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '4':
			RenderMeshIn2D(meshList[GEO_NO_4], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '5':
			RenderMeshIn2D(meshList[GEO_NO_5], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '6':
			RenderMeshIn2D(meshList[GEO_NO_6], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '7':
			RenderMeshIn2D(meshList[GEO_NO_7], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '8':
			RenderMeshIn2D(meshList[GEO_NO_8], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '9':
			RenderMeshIn2D(meshList[GEO_NO_9], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		case '0':
			RenderMeshIn2D(meshList[GEO_NO_0], false, m_worldWidth, m_worldHeight, scaleY, scaleY, posX, posY);
			break;
		default: break;
		}
	}
	if (buy_item == true)
	{
		if (purchased == true)
		{
			RenderMeshIn2D(meshList[GEO_BOUGHT], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 7.f));
		}
		else
		{
			RenderMeshIn2D(meshList[GEO_INSUFFICIENT], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.2f, m_worldHeight - (scaleY * 7.f));
			RenderMeshIn2D(meshList[GEO_GOLD], false, m_worldWidth, m_worldHeight, scaleX, scaleY, m_worldWidth * 0.325f, m_worldHeight - (scaleY * 7.f));
		}
	}
}