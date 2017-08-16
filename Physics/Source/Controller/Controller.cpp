#include "Controller.h"
#include "KeyboardController.h"

#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::istringstream;
using std::getline;
using std::cout;
using std::endl;

Controller::Controller()
	: m_player(nullptr)
	, kb_jump(0)
{
}

Controller::Controller(Player * _player)
{
	m_player = _player;
}

Controller::~Controller()
{
}

void Controller::Update(double dt)
{
	if (!m_player)
		return;

	if (KeyboardController::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		Move_LeftRight(dt, false);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_LEFT))
	{
		Move_LeftRight(dt, true);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(VK_SPACE))
	{
		Jump(dt);
	}
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

void Controller::SetPlayer(Player * _player)
{
	m_player = _player;
}

void Controller::Move_LeftRight(const double dt, const bool dLeft)
{
	m_player->Move_LeftRight(dt, dLeft);
}

void Controller::Jump(const double dt)
{
	m_player->Jump(dt);
}
