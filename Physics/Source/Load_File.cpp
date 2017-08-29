#include "Load_File.h"

Load_File::Load_File()
{
	Unit_Height_Space = 0;
	Unit_Width_Space = 0;
	Level = 0;
	Score = 0;
	Gold = 0;
	number_of_items = 0;
}

Load_File::~Load_File()
{
}

void Load_File::Init(vector<GameObject *>*Gameobj)
{
	Gameobject_List = Gameobj;
	blocks.Init();
}

bool Load_File::Load(bool checker, const string saveFileName)
{
	Unit_Height_Space = 0;
	ifstream myfile(saveFileName.c_str(), ios::in);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			istringstream ss(line);
			string content = "";
			while (getline(ss, content))
			{
				Process(checker, content);
			}
		}
		myfile.close();
	}
	else
	{
#if(_DEBUG == TRUE)
		cout << "PlayerInfo: Unable to load " << saveFileName.c_str() << endl;
#endif
		myfile.close();
		return false;
	}
	return true;
}

void Load_File::Process(bool checker, string content)
{
	string processor, item_name;
	int processed_value;
	int count = 0;
	int id = 0;
	int item_count = 0;
	int current = 0;
	int money = 0;
	int stat = 0;

	for (unsigned int i = 0; i < content.length(); i++)
	{
		if (content.at(i) == ',')
		{
			if (checker == false)
			{
				Unit_Width_Space += 5;
				processed_value = atoi(processor.c_str());
				blocks.Spawn(processed_value, Unit_Width_Space - 50, Unit_Height_Space, Gameobject_List);
			}
			else if (checker == true)
			{
				switch (current)
				{
					case 0:
					{
						item_name = processor;
						break;
					}
					case 1:
					{
						money = atoi(processor.c_str());
						break;
					}
					case 2:
					{
						has_item = (atoi(processor.c_str())) ? true : false;
						break;
					}
					case 3:
					{
						stat = atoi(processor.c_str());
						break;
					}
					case 4:
					{
						item_count = atoi(processor.c_str());
						break;
					}
				}
				current++;
			}
			processor = "";
		}
		else
		{
			processor.push_back(content.at(i));
		}
	}
	id = atoi(processor.c_str());
	if (checker == true)
	{
		items = items->insert(items, id, item_count, stat, has_item, money, item_name);
		items->setInstance(items);
		number_of_items++;
	}
	Unit_Height_Space += 5;
	Unit_Width_Space = 0;
}

bool Load_File::Load_Data(Tree::avl_node & node, const string saveFileName)
{

	ifstream myfile(saveFileName.c_str(), ios::in);
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			istringstream ss(line);
			string data = "";
			while (getline(ss, data, '='))
			{
				string theTag = data;
				getline(ss, data, '=');
				if (theTag == "Level")
				{
					Level = atoi(data.c_str());
				}
				else if (theTag == "Score")
				{
					Score = atoi(data.c_str());
				}
				else if (theTag == "Gold")
				{
					Gold = atoi(data.c_str());
				}
				else if (theTag == "item1")
				{
					if (node.id == 1)
					{
						node.has_item = (atoi(data.c_str())) ? true : false;
					}
				}
				else if (theTag == "item1_level")
				{
					if (node.id == 1)
					{
						node.item_count = atoi(data.c_str());
					}
				}
				else if (theTag == "item2")
				{
					if (node.id == 2)
					{
						node.has_item = atoi(data.c_str()) ? true : false;
					}
				}
				else if (theTag == "item2_level")
				{
					if (node.id == 2)
					{
						node.item_count = atoi(data.c_str());
					}
				}
				else if (theTag == "item3")
				{
					if (node.id == 3)
					{
						node.has_item = atoi(data.c_str()) ? true : false;
					}
				}
				else if (theTag == "item3_level")
				{
					if (node.id == 3)
					{
						node.item_count = atoi(data.c_str());
					}
				}
				else if (theTag == "item4")
				{
					if (node.id == 4)
					{
						node.has_item = atoi(data.c_str()) ? true : false;
					}
				}
				else if (theTag == "item4_level")
				{
					if (node.id == 4)
					{
						node.item_count = atoi(data.c_str());
					}
				}
			}
		}
		cout << Gold << endl;
		cout << "Loaded" << endl;
		items->root = node;
		items->setInstance(items);
		myfile.close();
	}
	else
	{
#if(_DEBUG == TRUE)
		cout << "PlayerInfo: Unable to load " << saveFileName.c_str() << endl;
#endif
		myfile.close();
		return false;
	}
	return true;
}

bool Load_File::Save_Data(int data1, int data2, int data3, Tree::avl_node & node, const string saveFileName)
{
	ofstream myfile;
	myfile.open(saveFileName.c_str(), ios::out | ios::ate);

	if (myfile.is_open())
	{
		myfile << "Level=" << data1 << endl;
		myfile << "Score=" << data2 << endl;
		myfile << "Gold=" << data3 << endl;
		if (node.id == 1)
		{
			myfile << "item1=" << node.has_item << endl;
			myfile << "item1_level=" << node.item_count << endl;
		}
		if (node.id == 2)
		{
			myfile << "item2=" << node.has_item << endl;
			myfile << "item2_level=" << node.item_count << endl;
		}
		if (node.id == 3)
		{
			myfile << "item3=" << node.has_item << endl;
			myfile << "item3_level=" << node.item_count << endl;
		}
		if (node.id == 4)
		{
			myfile << "item4=" << node.has_item << endl;
			myfile << "item4_level=" << node.item_count << endl;
		}
		cout << "saved" << endl;
		myfile.close();
		return true;
	}
	else
	{
#if(_DEBUG == TRUE)
		cout << "PlayerInfo: Unable to save " << saveFileName.c_str() << endl;
#endif
		myfile.close();
		return false;
	}
}

int Load_File::get_score()
{
	return Score;
}

int Load_File::get_gold()
{
	return Gold;
}