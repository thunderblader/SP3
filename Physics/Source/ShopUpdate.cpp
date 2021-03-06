#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Shop_Update(double dt)
{
	Gold = shop.get_gold();
	if (time_limit < 3)
	{
		time_limit += (float)dt;
	}
	if (time_limit >= 3)
	{
		buy_item = false;
	}
	if (move_shop == false)
	{
		if (item_id <= file.number_of_items - 1)
		{
			item_node = Tree::getInstance();
			item_id++;
			item_node = item_node->retreve_item(item_node, item_id);
		}
		move_shop = true;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		time_limit = 3;
		if (item_id >= 2)
		{
			item_node = Tree::getInstance();
			item_id--;
			item_node = item_node->retreve_item(item_node, item_id);
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		time_limit = 0;
		if (item_node->root.item_count >= 5)
		{

		}
		else
		{
			purchased = shop.Purchase_Item(item_id, item_node->root);
			buy_item = true;
			if (purchased)
			{
				item_node->root.has_item = true;
				item_node->root.item_count++;
				item_node->set_item(item_id);
				m_player->Upgrade(item_node->root);
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		time_limit = 3;
		if (item_id <= file.number_of_items - 1)
		{
			item_node = Tree::getInstance();
			item_id++;
			item_node = item_node->retreve_item(item_node, item_id);
		}
	}
}
