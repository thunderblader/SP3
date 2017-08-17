#include "Tree.h"

Tree::Tree()
{
	root = NULL;
//	rootss = NULL;
}

Tree::~Tree()
{
}

int Tree::height(avl_node *temp)
{
	int h = 0;
	if (temp != NULL)
	{
		int l_height = height(temp->left);
		int r_height = height(temp->right);
		int max_height = max(l_height, r_height);
		h = max_height + 1;
	}
	return h;
}

int Tree::diff(avl_node *temp)
{
	int l_height = height(temp->left);
	int r_height = height(temp->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

Tree::avl_node *Tree::rr_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

Tree::avl_node *Tree::ll_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

Tree::avl_node *Tree::lr_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = rr_rotation(temp);
	return ll_rotation(parent);
}

Tree::avl_node *Tree::rl_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = ll_rotation(temp);
	return rr_rotation(parent);
}

Tree::avl_node *Tree::balance(avl_node *temp)
{
	int bal_factor = diff(temp);
	if (bal_factor > 1)
	{
		if (diff(temp->left) > 0)
			temp = ll_rotation(temp);
		else
			temp = lr_rotation(temp);
	}
	else if (bal_factor < -1)
	{
		if (diff(temp->right) > 0)
			temp = rl_rotation(temp);
		else
			temp = rr_rotation(temp);
	}
	return temp;
}

Tree::avl_node *Tree::insert(avl_node *root, int value, string name)
{
	if (root == NULL)
	{
		root = new avl_node;
		root->name = name;
		root->data = value;	//here
		root->left = NULL;
		root->right = NULL;
		return root;
	}
	else if (value < root->data)
	{
		root->left = insert(root->left, value, name);
		root = balance(root);
	}
	else if (value >= root->data)
	{
		root->right = insert(root->right, value, name);
		root = balance(root);
	}
	return root;
}
void Tree::inorder(avl_node *tree)
{
	if (tree == NULL)
	{
		return;
	}
	inorder(tree->left);
	cout << tree->data << ", ";
	inorder(tree->right);
}


