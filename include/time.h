#ifndef TIME_H
#define TIME_H

#include <GLFW/glfw3.h>

class Time
{
	float time = 0.0f;
	float lTime = 0.0f;
	int frames = 0;
	
public:
	static float dt;
	static int FPS;
	static float cTime;
	
	static void calculate();
};

#endif
