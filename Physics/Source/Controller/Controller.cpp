#include "Controller.h"
#include "../Application.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::getline;
using std::cout;
using std::endl;

Controller::Controller() : kb_jump(0)
{
}

Controller::~Controller()
{
}

void Controller::Update(double dt)
{

}

bool Controller::LoadConfig(const string filePath)
{
	ifstream file(filePath.c_str());

	if (file.is_open())
	{
		string line;

		while (getline(file, line))
		{
			istringstream ss(line);
			string tag = "";
			string token = "";

			getline(ss, tag, '=');
			getline(ss, token, '\n');

			if (tag == "Jump")
				kb_jump = VK_SPACE;
		}

		file.close();
		return true;
	}
	else
	{
		cout << "Controller: Unable to load Config file at \"" << filePath << "\"." << endl;
		file.close();
		return false;
	}
}

void Controller::SetPlayer(Player * _m_player)
{
	m_player = _m_player;
}

char Controller::GetJumpKey()
{
	return kb_jump;
}
