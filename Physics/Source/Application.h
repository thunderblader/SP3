
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();

	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);

	static int GetWindowWidth();
	static int GetWindowHeight();

private:
	Application();
	~Application();

	static bool IsKeyPressed(unsigned short key);

	//Declare a window object
	StopWatch m_timer;
};

#endif