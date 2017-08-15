#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <Windows.h>

using std::string;

class Controller
{
public:
	Controller();
	~Controller();

	bool LoadConfig(const string filePath);


	char GetJumpKey();

private:
	char kb_jump;
};

#endif // !CONTROLLER_H
