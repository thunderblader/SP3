#include "Shop.h"

void Shop::Load_Shop()
{
	
//	shop_file.Load("shop_data");
//	itemsss = new Tree;
	Tree::getInstance()->inorder(Tree::getInstance());
	gold = 1000;

}

void Shop::Purchase_Item(int test)
{
	//items = NULL;
	
	//items = items->insert(items, 1, 10, "apple");
	//items = items->insert(items, 2, 50, "popcorn");
	//items = items->insert(items, 3, 100, "gun");
	//items = items->insert(items, 4, 1000, "diamond");


//	itemsss = itemsss->Get_Tree();

//	itemsss->inorder(itemsss);

	//itemsss = shop_file.Get_Tree();




	//cout << "gold: " << gold << endl;
	//cout << "Which do u want to buy?" << endl;
	//if (test == 1)
	//{
	//	//cout << "bought " << Tree::getInstance()->root.name << endl;
	//	//cout << "bought " << 
	//	Tree::getInstance()->search(Tree::getInstance(), 1, 0);// << endl;
	//	//gold -= Tree::getInstance()->root.price;
	//	//cout << "gold: " << gold << endl;
	//}
	//else if (test == 2)
	//{
	//	Tree::getInstance()->search(Tree::getInstance(), 50, 0);
	//	/*cout << "bought " << Tree::getInstance()->root.name << endl;
	//	gold -= Tree::getInstance()->root.price;
	//	cout << "gold: " << gold << endl;*/
	//}
	//else if (test == 3)
	//{
	//	Tree::getInstance()->search(Tree::getInstance(), 100, 0);
	//	/*cout << "bought " << Tree::getInstance()->root.name << endl;
	//	gold -= Tree::getInstance()->root.price;
	//	cout << "gold: " << gold << endl;*/
	//}
	//else if (test == 4)
	//{
	//	Tree::getInstance()->search(Tree::getInstance(), 1000, 0);
	//	/*cout << "bought " << Tree::getInstance()->root.name << endl;
	//	gold -= Tree::getInstance()->root.price;
	//	cout << "gold: " << gold << endl;*/
	//}


}

void Shop::Purchase_Upgrade()
{

}