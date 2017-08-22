#include "Shop.h"

void Shop::Load_Shop()
{
	gold = 1000;
	item_price = 0;
	
}

bool Shop::Purchase_Item(int id)
{
	item_price = 0;
	stuff = Tree::getInstance();
	stuff = stuff->retreve_item(stuff, id);
//	cout << "gold: " << gold << endl;
//	cout << "Which do u want to buy?" << endl;
	cout << "count: " << stuff->root.item_count << endl;
	
	if (gold < stuff->root.price)
	{			
	//	cout << "you no money" << endl;
	//	cout << "cost is: " << item_price << endl;
		return false;
	}
	else
	{
		gold -= stuff->root.price;
	//	cout << "gold: " << gold << endl;
	//	cout << "you bought: " << stuff->root.name << endl;
		return true;
	}
}

void Shop::Purchase_Upgrade(int id)
{
	stuff = Tree::getInstance();
	stuff = stuff->retreve_item(stuff, id);
	//cout << stuff->root.data << endl;
	//cout << stuff->root.name << endl;
	//cout << stuff->root.price << endl;
}
void Shop::get_item(int id)
{
	stuff = stuff->retreve_item(stuff, id);
}