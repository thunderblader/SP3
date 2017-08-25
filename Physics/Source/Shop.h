#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include "Load_File.h"
#include "Tree.h"

using namespace std;

class Shop
{
public:
	void Load_Shop();
	void Add_gold(int Add);
	bool Purchase_Item(int id);
	void get_item(int id);
	
	int gold;
	int item_price;

	Load_File shop_file;
	Tree *stuff;
	
	

};

#endif