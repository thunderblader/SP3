#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
//#include "Scene_Test.h"
#include "Scene01.h"


using namespace std;

class Load_File
{
public:
//	Load_File();
//	~Load_File();

	void Init();
	bool Load(const string saveFileName = "Image//Test_Level.csv");
	void Process(string content);
	bool Load_Data(const string saveFileName = "Image//test.sav");
	bool Save_Data(int data1, int data2, int data3, const string saveFileName = "Image//test.sav");

protected:
	int Unit_Height_Space;
	int Unit_Width_Space;
	int Level;
	int Score;
	int Gold;
};

#endif