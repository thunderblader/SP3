#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "Spawner.h"
//#include "Scene01.h"


using namespace std;

class Load_File
{
public:
	Load_File();
	~Load_File();

	void Init(vector<GameObject *>*Gameobj);
	bool Load(const string saveFileName);
	void Process(string content);
	bool Load_Data(const string saveFileName = "Image//test.sav");
	bool Save_Data(int data1, int data2, int data3, const string saveFileName = "Image//test.sav");

protected:
	vector<GameObject *>* Gameobject_List;
	Spawner object;
	Spawner pop;
	int Unit_Height_Space;
	int Unit_Width_Space;
	int Level;
	int Score;
	int Gold;
	
};

#endif