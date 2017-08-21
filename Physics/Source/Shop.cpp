#include "Shop.h"

void Shop::Load_Shop()
{
	
//	shop_file.Load("shop_data");
	gold = 1000;
	item_price = 0;
}

void Shop::Purchase_Item(int id)
{
	item_price = 0;
	stuff = Tree::getInstance();
	cout << "gold: " << gold << endl;
	cout << "Which do u want to buy?" << endl;
	cout << "you bought: ";
	item_price = Tree::getInstance()->search(stuff, id);
	if (gold < item_price)
	{			
		cout << "you no money" << endl;
		cout << "cost is: " << item_price << endl;
	}
	else
	{
		gold -= item_price;
		cout << "gold: " << gold << endl;
	}
}

void Shop::Purchase_Upgrade()
{

}
void Shop::get_item(int id)
{
	item_price = Tree::getInstance()->search(stuff, id);
	cout << "Price: " << item_price << endl;
}