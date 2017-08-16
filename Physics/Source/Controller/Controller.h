#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../Player.h"

#include <string>
#include <Windows.h>

using std::string;

class Controller
{
public:
	Controller();
	Controller(Player* _player);
	~Controller();

	void Update(const double dt);

	bool LoadConfig(const string filePath);

	void SetPlayer(Player* _player);

	void Move_LeftRight(const double dt, const bool dLeft);

private:
	char kb_jump;

	Player* m_player;
};

#endif // !CONTROLLER_H
