#include "Shop.h"

void Shop::Load_Shop()
{
	gold = 1000;
}

void Shop::Add_gold(int Add)
{
	gold += Add;
}

bool Shop::Purchase_Item(int id, Tree::avl_node &node)
{
	if (node.id == 1)
	{
		if (gold < node.price)
		{
			return false;
		}
		else
		{
			gold -= node.price;
			return true;
		}
	}
	if (node.id == 2)
	{
		if (gold < node.price)
		{
			return false;
		}
		else
		{
			gold -= node.price;
			return true;
		}
	}
	if (node.id == 3)
	{
		if (gold < node.price)
		{
			return false;
		}
		else
		{
			gold -= node.price;
			return true;
		}
	}
	if (node.id == 4)
	{
		if (gold < node.price)
		{
			return false;
		}
		else
		{
			gold -= node.price;
			return true;
		}
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

int Shop::get_gold()
{
	return gold;
}