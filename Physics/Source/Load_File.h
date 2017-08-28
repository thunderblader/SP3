#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "Spawner.h"
#include "Tree.h"

using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::endl;

class Load_File
{
public:
	Load_File();
	~Load_File();

	void Init(vector<GameObject *>*Gameobj);
	bool Load(bool checker, const string saveFileName);
	void Process(bool checker, string content);
	bool Load_Data(Tree::avl_node & node, const string saveFileName = "Image//save_data.sav");
	bool Save_Data(int data1, int data2, int data3, Tree::avl_node &node, const string saveFileName = "Image//save_data.sav");
	int get_score();
	int get_gold();

	int number_of_items;

protected:
	vector<GameObject *>* Gameobject_List;
	Tree *items = Tree::getInstance();
	Spawner blocks;
	int Unit_Height_Space;
	int Unit_Width_Space;
	int Level;
	int Score;
	int Gold;
	bool has_item;

};

#endif