#include <GLFW/glfw3.h>

#include <map>
#include <string>

using namespace std;

bool pressedKeys[512]{};

map<string, int> keys
{
	{"FORWARD",	GLFW_KEY_W},
	{"BACK",	GLFW_KEY_S},
	{"LEFT",	GLFW_KEY_A},
	{"RIGHT",	GLFW_KEY_D},
	{"JUMP",	GLFW_KEY_SPACE},
	{"ATTACK",	GLFW_KEY_ENTER}, /**/
	{"DROP",	GLFW_KEY_G},
	{"SET",		GLFW_KEY_E},
	{"SWAP",	GLFW_KEY_Q},
	{"MODE",	GLFW_KEY_F},
	
	{"SCORE",	GLFW_KEY_TAB},
};

map<string, int> buttons
{
	{"ATTACK",	GLFW_MOUSE_BUTTON_LEFT}, 
};

void ChangeKey(const string& name, unsigned int code);