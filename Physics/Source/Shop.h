#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include "Load_File.h"

using namespace std;

class Shop
{
public:
	void Load_Shop();
	void Purchase_Item();
	void Purchase_Upgrade();

	struct Items
	{
		int item1;
		int item2;
		int item3;
	};

	Load_File shop_file;


};

#endif