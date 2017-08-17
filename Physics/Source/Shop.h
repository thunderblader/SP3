#ifndef SHOP_H
#define SHOP_H

#include <iostream>
//#include "Load_File.h"
#include "Tree.h"

using namespace std;

class Shop
{
public:
	void Load_Shop();
	void Purchase_Item();
	void Purchase_Upgrade();


	//Load_File shop_file;
	//Tree *items;
	Tree itemssss;


};

#endif