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
	~Controller();

	void Update(double dt);

	bool LoadConfig(const string filePath);

	void SetPlayer(Player* _m_player);

	char GetJumpKey();

private:
	char kb_jump;

	Player* m_player;
};

#endif // !CONTROLLER_H
