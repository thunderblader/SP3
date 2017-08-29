#ifndef TREE_H
#define TREE_H
#define max(a,b)		(((a) > (b)) ? (a) : (b))

#include "MemoryLeakCheck.h"
#include <iostream>
#include <string>

using namespace std;

class Tree
{
private:
	static Tree *node;
	Tree();

public:
	struct avl_node
	{
		string name;
		int id;
		int item_count;
		int data;
		int price;
		bool has_item;	
	};
	
	~Tree();

	int height(Tree *);
	int diff(Tree *);
	Tree *rr_rotation(Tree *);
	Tree *ll_rotation(Tree *);
	Tree *lr_rotation(Tree *);
	Tree *rl_rotation(Tree *);
	Tree* balance(Tree *);
	Tree* insert(Tree *, int data, int item_count, int stat, bool has_item, int price, string name);
	void inorder(Tree *);
	Tree* retreve_item(Tree*, int value);
	void set_item(int id);

	static Tree *getInstance();
	void setInstance(Tree *);

	int new_gold;
	bool check;

	Tree* temp;
	Tree *create;
	Tree *left;
	Tree *right;
	Tree *curr;
	avl_node root;


};

#endif