#include "Load_File.h"



bool Load_File::Load(const string saveFileName)
{
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
				Process(content);
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

void Load_File::Process(string content)
{
	string processor;
	int processed_value;

	for (int i = 0; i < content.length(); i++)
	{
		if (content.at(i) == ',')
		{
			Scene_Test object;
			Unit_Width_Space += 5;
			processed_value = atoi(processor.c_str());
			object.Spawn(processed_value, Unit_Width_Space, Unit_Width_Space);
			processor = "";
		}
		else
		{
			processor.push_back(content.at(i));
		}
	}
	Unit_Height_Space += 5;
	Unit_Width_Space = 0;
}

bool Load_File::Load_Data(const string saveFileName)
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
			}
		}
		cout << Level << endl;
		cout << Score << endl;
		cout << Gold << endl;
		cout << "Loaded" << endl;
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

bool Load_File::Save_Data(int data1, int data2, int data3, const string saveFileName)
{
	ofstream myfile;
	myfile.open(saveFileName.c_str(), ios::out | ios::ate);

	if (myfile.is_open())
	{
		myfile << "Level=" << data1 << endl;
		myfile << "Score=" << data2 << endl;
		myfile << "Gold=" << data3 << endl;
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