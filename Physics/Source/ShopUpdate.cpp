#include "Scene01.h"
#include "KeyboardController.h"

void Scene01::Shop_Update(double dt)
{
	if (time_limit < 3)
	{
		time_limit += dt;
	}
	if (time_limit >= 3)
	{
		buy_item = false;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		if (item_id >= 1)
		{
			item_node = Tree::getInstance();
			item_id--;
			item_node = item_node->retreve_item(item_node, item_id);
			cout << item_node->root.name << endl;
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		time_limit = 0;
		if (item_node->root.has_item == true)
		{

		}
		else
		{
			purchased = shop.Purchase_Item(item_id);
			buy_item = true;
			if (purchased)
			{
				item_node->root.has_item = true;
				item_node->set_item(item_id);
				m_player->Upgrade(item_id);
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		if (item_id <= file.number_of_items - 1)
		{
			item_node = Tree::getInstance();
			item_id++;
			item_node = item_node->retreve_item(item_node, item_id);
			cout << item_node->root.name << endl;
			cout << item_node->root.has_item << endl;
		}
	}
}