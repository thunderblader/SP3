#ifndef TREE_H
#define TREE_H
#define max(a,b)		(((a) > (b)) ? (a) : (b))

#include <iostream>

using namespace std;





class Tree
{
public:
	struct avl_node
	{
		string name;	//name of item
		int data;	//data,price,stats
		bool check;	//check if player has item


		struct avl_node *left;
		struct avl_node *right;
	}*root;

	avl_node rootss;

	Tree();
	~Tree();

	int height(avl_node *);
	int diff(avl_node *);
	avl_node *rr_rotation(avl_node *);
	avl_node *ll_rotation(avl_node *);
	avl_node *lr_rotation(avl_node *);
	avl_node *rl_rotation(avl_node *);
	avl_node* balance(avl_node *);
	avl_node* insert(avl_node *, int data, string name);
	void inorder(avl_node *);






};

#endif