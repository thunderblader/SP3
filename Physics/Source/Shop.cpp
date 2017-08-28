#include "Shop.h"

void Shop::Load_Shop()
{
	gold = 1000;
	item_price = 0;
}

void Shop::Add_gold(int Add)
{
	gold += Add;
}

bool Shop::Purchase_Item(int id)
{
	item_price = 0;
	stuff = Tree::getInstance();
	stuff = stuff->retreve_item(stuff, id);
	if (gold < stuff->root.price)
	{			
		return false;
	}
	else
	{
		gold -= stuff->root.price;
		return true;
	}
}

void Shop::get_item(int id)
{
	stuff = stuff->retreve_item(stuff, id);
}

void Shop::set_gold(int new_gold)
{
	gold = new_gold;
}