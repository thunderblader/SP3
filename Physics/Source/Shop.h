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
	bool Purchase_Item(int id, Tree::avl_node &node);
	void get_item(int id);
	void set_gold(int new_gold);
	int get_gold();
	
	int gold;

	Load_File shop_file;
	Tree *stuff;
};

#endif