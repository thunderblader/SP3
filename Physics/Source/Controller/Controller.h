#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../Player.h"

#include <string>
#include <Windows.h>

using std::string;

struct ParamPhysics;

class Controller
{
public:
	Controller();
	Controller(Player* _player);
	~Controller();

	void Update(const double dt);

	bool LoadConfig(const string filePath, ParamPhysics& _paramPhysics);

	void SetPlayer(Player* _player);

	void Move_LeftRight(const double dt, const bool dLeft);
	void Jump(const double dt);

private:
	char kb_jump, kb_left, kb_right;

	Player* m_player;
};

#endif // !CONTROLLER_H
