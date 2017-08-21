#include "Tree.h"

Tree::Tree()
{
	check = false;
	root.data = NULL;
	curr = NULL;
}

Tree::~Tree()
{
}
Tree *Tree::node = NULL;

Tree *Tree::getInstance()
{
	if (node == NULL)
	{
		node = new Tree();
	}
	return node;
}

void Tree::setInstance(Tree* input)
{
	node = input;
}

int Tree::height(Tree *temp)
{
	int h = 0;
	if (temp == NULL)
	{

	}
	else if (temp != NULL)
	{
		int l_height = height(temp->left);
		int r_height = height(temp->right);
		int max_height = max(l_height, r_height);
		h = max_height + 1;
	}
	
	return h;
}

int Tree::diff(Tree *temp)
{
	int l_height = height(temp->left);
	int r_height = height(temp->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

Tree *Tree::rr_rotation(Tree *parent)
{
	Tree *temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

Tree *Tree::ll_rotation(Tree *parent)
{
	Tree *temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

Tree *Tree::lr_rotation(Tree *parent)
{
	Tree *temp;
	temp = parent->left;
	parent->left = rr_rotation(temp);
	return ll_rotation(parent);
}

Tree *Tree::rl_rotation(Tree *parent)
{
	Tree *temp;
	temp = parent->right;
	parent->right = ll_rotation(temp);
	return rr_rotation(parent);
}

Tree *Tree::balance(Tree *temp)
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

Tree* Tree::insert(Tree *oak, int value, int price, string name)
{
	curr = oak;
	bool check = false;
	create = new Tree;
	create->root.name = name;
	create->root.data = value;
	create->root.price = price;
	while (check == false)
	{
		if (curr->root.data == NULL)
		{
			curr = create;
			curr->left = NULL;
			curr->right = NULL;
			oak = curr;
			check = true;

			return oak;
		}
		else if (value < curr->root.data)
		{
			if (oak->left == NULL)
			{
				oak->left = create;
				curr = curr->left;
				curr->right = NULL;
				curr->left = NULL;
				check = true;
			}
			else
			{
				if (curr->left == NULL)
				{
					curr->left = create;
					curr = curr->left;
					curr->right = NULL;
					curr->left = NULL;
					check = true;
				}
				else
				{
					curr = curr->left;
				}
			}
			oak = balance(oak);
		}
		else if (value >= curr->root.data)
		{
			if (oak->right == NULL)
			{
				oak->right = create;
				curr = curr->right;
				curr->right = NULL;
				curr->left = NULL;
				check = true;
			}
			else
			{
				if (curr->right == NULL)
				{
					curr->right = create;
					curr = curr->right;
					curr->right = NULL;
					curr->left = NULL;
					check = true;
				}
				else
				{
					curr = curr->right;
				}
			}
			oak = balance(oak);
		}
	}
	//birch = oak;
	return oak;
}
void Tree::inorder(Tree *tree)
{
	if (tree == NULL)
	{
		return;
	}
	inorder(tree->left);
	cout << tree->root.name << ", ";
	cout << tree->root.price << ", ";
	cout << tree->root.data << endl;
	inorder(tree->right);
}
int Tree::search(Tree *tree, int value)
{
	if (tree == NULL)
	{
		if (check == true)
		{
			return new_gold;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		search(tree->left, value);
		if (value == tree->root.data)
		{
			cout << tree->root.name << endl;
			new_gold = tree->root.price;
			check = true;
			return tree->root.price;
		}
		search(tree->right, value);
	}


}


