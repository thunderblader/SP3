#include "Shop.h"

void Shop::Load_Shop()
{
	
//	shop_file.Load("shop_data");

}

void Shop::Purchase_Item()
{
	Tree *items;
	items = new Tree;



	//items = items->insert(items->root, 1, "hihihh");
	//items = new Tree;
	items->insert(items->root, 2, "lololol");
	items->insert(items->root, 3, "456");
	items->inorder(items->root);

	//itemssss.insert(itemssss.root, 1, "hihihh");
	//itemssss.insert(itemssss.root, 2, "lololol");
	//itemssss.insert(itemssss.root, 3, "456");
	//itemssss.inorder(itemssss.rootss);
	
}

void Shop::Purchase_Upgrade()
{

}