#ifndef TREE_H
#define TREE_H
#define max(a,b)		(((a) > (b)) ? (a) : (b))

#include <iostream>
#include <string>

using namespace std;

class Tree
{
private:
	static Tree *test;
	Tree();

public:
	struct avl_node
	{
		string name;	//name of item
		int data;	//data,price,stats
		int price;
		//bool check;	//check if player has item
	};
	
	~Tree();

	int height(Tree *);
	int diff(Tree *);
	Tree *rr_rotation(Tree *);
	Tree *ll_rotation(Tree *);
	Tree *lr_rotation(Tree *);
	Tree *rl_rotation(Tree *);
	Tree* balance(Tree *);
	Tree* insert(Tree *, int data, int price, string name);
	void inorder(Tree *);
	void search(Tree *, int value, int price);

	static Tree *getInstance();
	void setInstance(Tree *);

	int gold = 1000;

	
	Tree *create;
	Tree *left;
	Tree *right;
	Tree *curr;
	avl_node root;


};

#endif