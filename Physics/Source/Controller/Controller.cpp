#include "Controller.h"
#include "KeyboardController.h"
#include "../ParameterPhysics.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::ifstream;
using std::istringstream;
using std::getline;
using std::remove;
using std::cout;
using std::endl;

Controller::Controller()
	: m_player(nullptr)
	, kb_jump(0)
	, kb_left(0)
	, kb_right(0)
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

	if (KeyboardController::GetInstance()->IsKeyDown(kb_left))
	{
		Move_LeftRight(dt, true);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(kb_right))
	{
		Move_LeftRight(dt, false);
	}
	if (KeyboardController::GetInstance()->IsKeyDown(kb_jump))
	{
		Jump(dt);
	}
}

bool Controller::LoadConfig(const string filePath, ParamPhysics& _paramPhysics)
{
	ifstream file(filePath.c_str());

	if (file.is_open())
	{
		string line;
		string category;

		while (getline(file, line))
		{
			istringstream ss(line);
			string tag = "";
			string token = "";

			getline(ss, tag, '=');

			if (tag[0] == '/' || tag == "")
				continue;
			else if (tag[0] == '#')
			{
				category = tag;
				continue;
			}

			getline(ss, token, '\n');

			tag.erase(remove(tag.begin(), tag.end(), ' '), tag.end());
			token.erase(remove(token.begin(), token.end(), ' '), token.end());

			if (category == "#Keyboard")
			{
				if (tag == "Jump")
					kb_jump = atoi(token.c_str());
				else if (tag == "MoveLeft")
					kb_left = atoi(token.c_str());
				else if (tag == "MoveRight")
					kb_right = atoi(token.c_str());
			}
			else if (category == "#Physics")
			{
				if (tag == "Gravity")
					_paramPhysics.gravity = strtof(token.c_str(), 0);
				else if (tag == "AirFriction")
					_paramPhysics.frictionAir = strtof(token.c_str(), 0);
				else if (tag == "TerrainFriction")
					_paramPhysics.frictionTerrain = strtof(token.c_str(), 0);
				else if (tag == "CartMass")
					_paramPhysics.massCart = strtof(token.c_str(), 0);
				else if (tag == "Acceleration")
					_paramPhysics.acceleration = strtof(token.c_str(), 0);
				else if (tag == "SpeedLimit")
					_paramPhysics.speedLimit = strtof(token.c_str(), 0);
			}
		}

		file.close();
		return true;
	}
	else
	{
		cout << "Controller: Unable to load Config file at \"" << filePath << "." << endl;
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
