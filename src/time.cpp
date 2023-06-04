#include "../include/time.h"

float Time::dt = 0.0f;
int Time::FPS = 0;
float Time::cTime = 0.0f;

void Time::calculate()
{
	static float time = 0.0f;
	static float lTime = 0.0f;
	static int frames = 0;		
	
	/*dt*/
	cTime = glfwGetTime();
//	cTime = cTime + 0.01f/2;
	dt = cTime - lTime;
	lTime = cTime;
	
	/*fps*/
	time += dt;
	frames++;
	if(time >= 1.0)
	{
		FPS = frames;
		time -= 1.0f;
		frames = 0;
	}
}