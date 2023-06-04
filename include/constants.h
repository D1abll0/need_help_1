#ifndef CONSTANTS_H
#define CONSTANTS_H

#include<glm/glm.hpp>
#include<string>
#include<map>

using namespace std;
using namespace glm;

/*define*/




/*enum*/
enum FORM {ELLIPSOID, HAMMER, TETRAMINO};
enum class TETRAMINO_TYPE{NONE = 0, I, O, S, L, T};
enum class TETRAMINO_MODE{NONE, JUMP, SET};
enum class MOVEMENT {NONE = 0, FORWARD = 1, BACK = -1, LEFT = -1, RIGHT = 1};

enum class MDLType {STATIC, DYNAMIC};
enum Color 
{
	Auburn, BlueJeans, BlueViolet, ChineseBrown, DarkLiver, DarkViolet, HarlequinGreen, ImperialPurple, IndiaGreen, JordyBlue, Magenta, Marigold, PinkFlamingo, SheenGreen, UFOGreen, UltramarineBlue
};

enum Camera_Movement
{
	FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN,
};

enum LightCaster
{
	Direction,
	Point,
	Spot
};



/*struct*/
struct JumpLength
{
	int x, z;
};

struct Cells
{
	int da;
	vec2 c1, c2, c3;
	float angle;
	int scaleX, scaleZ;
	vec2 dpos;
};

struct Cell
{
	TETRAMINO_TYPE tetraminoPart = TETRAMINO_TYPE::NONE;
	bool isBusy;
};

/*var*/
const vec3 Colors[]
{
	vec3(0.63, 0.14, 0.12), //Auburn
	vec3(0.39, 0.68, 1), //BlueJeans
	vec3(0.45, 0.16, 1), //Blue-Violet
	vec3(0.71, 0.19, 0.13), //ChineseBrown
	vec3(0.31, 0.31, 0.31), //DarkLiver
	vec3(0.61, 0.09, 0.81), //DarkViolet
	vec3(0.35, 0.91, 0.19), //HarlequinGreen
	vec3(0.42, 0, 	 0.26), //ImperialPurple
	vec3(0.06, 0.58, 0), //IndiaGreen
	vec3(0.58, 0.58, 1), //JordyBlue
	vec3(0.71, 0.13, 0.48), //Magenta
	vec3(0.91, 0.61, 0.13), //Marigold
	vec3(0.94, 0.42, 1), //PinkFlamingo
	vec3(0.55, 0.84, 0), //SheenGreen
	vec3(0.26, 0.87, 0.52), //UFOGreen
	vec3(0.26, 0.26, 1), //UltramarineBlue
};

const map<int, JumpLength> jumpsByAngleSL
{
	{ 30, 	{-1, -2} },
	{ 60, 	{-2, -1} },
	{ 120, 	{-2,  1} },
	{ 150, 	{-1,  2} },
	{ 210, 	{ 1,  2} },
	{ 240, 	{ 2,  1} },
	{ 300, 	{ 2, -1} },
	{ 330, 	{ 1, -2} }
};

const map<int, JumpLength> jumpsByAngleT
{
	{ 0, 	{ 0, -2} },
	{ 45, 	{-1, -1} },
	{ 90, 	{-2,  0} },
	{ 135, 	{-1,  1} },
	{ 180, 	{ 0,  2} },
	{ 225, 	{ 1,  1} },
	{ 270, 	{ 2,  0} },
	{ 315, 	{ 1, -1} },
};

const map<int, JumpLength> jumpsByAngleI
{
	{ 0, 	{ 0, -3} },
	{ 90, 	{-3,  0} },
	{ 180, 	{ 0,  3} },
	{ 270, 	{ 3,  0} }
};

const map<int, JumpLength> jumpsByAngleO
{
	{ 0, 	{ 0, -1} },
	{ 45, 	{-1, -1} },
	{ 90, 	{-1,  0} },
	{ 135, 	{-1,  1} },
	{ 180, 	{ 0,  1} },
	{ 225, 	{ 1,  1} },
	{ 270, 	{ 1,  0} },
	{ 315, 	{ 1, -1} }
};

const multimap<float, Cells> setPositionsByAngleI
{
	{0, 	Cells{ 0,	vec2(-3,  0), vec2(-1,  0), vec2(-2,  0), 0,	 1,  1,	vec2( 0,  0)} },
	{90, 	Cells{ 0,	vec2( 0, -3), vec2( 0, -1), vec2( 0, -2), 90,	 1,  1,	vec2( 0,  0)} },
	{180, 	Cells{ 0,	vec2( 3,  0), vec2( 1,  0), vec2( 2,  0), 180,	 1,  1,	vec2( 0,  0)} },
	{270, 	Cells{ 0,	vec2( 0,  3), vec2( 0,  1), vec2( 0,  2), 270,	 1,  1,	vec2( 0,  0)} },
};

const multimap<float, Cells> setPositionsByAngleO
{
	{45, 	Cells{ 0,	vec2(-1, -1), vec2(-1,  0), vec2( 0, -1), 90, 	 1,  1,	vec2( 0,  0)} },
	{135, 	Cells{ 0,	vec2( 1, -1), vec2( 0, -1), vec2( 1,  0), 180,	 1,  1,	vec2( 0,  0)} },
	{225, 	Cells{ 0,	vec2( 1,  1), vec2( 1,  0), vec2( 0,  1), 270,	 1,  1,	vec2( 0,  0)} },
	{315, 	Cells{ 0,	vec2(-1,  1), vec2( 0,  1), vec2(-1,  0), 0,	 1,  1,	vec2( 0,  0)} },
};

//const multimap<float, Cells> setPositionsByAngleT
//{
//	{0, 	Cells{-1,	vec2(-2,  0), vec2(-1, -1), vec2(-1,  0), 0,	 1,  1,	vec2( 0,  0)} },
//	{0, 	Cells{ 1,	vec2(-2,  0), vec2(-1,  1), vec2(-1,  0), 0,	-1,  1,	vec2( 0,  0)} },
//	{90, 	Cells{-1,	vec2( 0, -2), vec2( 1, -1), vec2( 0, -1), 90,	 1,  1,	vec2( 0,  0)} },
//	{90, 	Cells{ 1,	vec2( 0, -2), vec2(-1, -1), vec2( 0, -1), 90,	-1,  1,	vec2( 0,  0)} },
//	{180, 	Cells{-1,	vec2( 2,  0), vec2( 1,  1), vec2( 1,  0), 180,	 1,  1,	vec2( 0,  0)} },
//	{180, 	Cells{ 1,	vec2( 2,  0), vec2( 1, -1), vec2( 1,  0), 180,	-1,  1,	vec2( 0,  0)} },
//	{270, 	Cells{-1,	vec2( 0,  2), vec2(-1,  1), vec2( 0,  1), 270,	 1,  1,	vec2( 0,  0)} },
//	{270, 	Cells{ 1,	vec2( 0,  2), vec2( 1,  1), vec2( 0,  1), 270,	-1,  1,	vec2( 0,  0)} },
//};

const multimap<float, Cells> setPositionsByAngleT
{
	{0, 	Cells{ 1,	vec2(-2,  0), vec2(-1, -1), vec2(-1,  0), 0,	 -1,  1,	vec2( 0,  0)} },
	{0, 	Cells{-1,	vec2(-2,  0), vec2(-1,  1), vec2(-1,  0), 0,	1,  1,	vec2( 0,  0)} },
	{90, 	Cells{ 1,	vec2( 0, -2), vec2( 1, -1), vec2( 0, -1), 90,	 -1,  1,	vec2( 0,  0)} },
	{90, 	Cells{-1,	vec2( 0, -2), vec2(-1, -1), vec2( 0, -1), 90,	1,  1,	vec2( 0,  0)} },
	{180, 	Cells{ 1,	vec2( 2,  0), vec2( 1,  1), vec2( 1,  0), 180,	 -1,  1,	vec2( 0,  0)} },
	{180, 	Cells{-1,	vec2( 2,  0), vec2( 1, -1), vec2( 1,  0), 180,	1,  1,	vec2( 0,  0)} },
	{270, 	Cells{ 1,	vec2( 0,  2), vec2(-1,  1), vec2( 0,  1), 270,	 -1,  1,	vec2( 0,  0)} },
	{270, 	Cells{-1,	vec2( 0,  2), vec2( 1,  1), vec2( 0,  1), 270,	1,  1,	vec2( 0,  0)} },
};

const multimap<float, Cells> setPositionsByAngleL
{
	{30, 	Cells{-1,	vec2(-2, -1), vec2(-1,  0), vec2(-2,  0), 0,	-1,  1,	vec2( 0,  0)} },
	{30, 	Cells{ 1,	vec2(-2, -1), vec2( 0, -1), vec2(-1, -1), 0,	 1, -1,	vec2(-2, -1)} },
	{60, 	Cells{-1,	vec2(-1, -2), vec2(-1,  0), vec2(-1, -1), 270,	 1,  1,	vec2(-1, -2)} },
	{60, 	Cells{ 1,	vec2(-1, -2), vec2( 0, -1), vec2( 0, -2), 90,	 1,  1,	vec2( 0,  0)} },
	{120, 	Cells{-1,	vec2( 1, -2), vec2( 0, -1), vec2( 0, -2), 90,	-1,  1,	vec2( 0,  0)} },
	{120, 	Cells{ 1,	vec2( 1, -2), vec2( 1,  0), vec2( 1, -1), 90,	 1, -1,	vec2( 1, -2)} },
	{150, 	Cells{-1,	vec2( 2, -1), vec2( 0, -1), vec2( 1, -1), 0,	 1,  1,	vec2( 2, -1)} },
	{150, 	Cells{ 1,	vec2( 2, -1), vec2( 1,  0), vec2( 2,  0), 0,	-1, -1,	vec2( 0,  0)} },
	{210, 	Cells{-1,	vec2( 2,  1), vec2( 1,  0), vec2( 2,  0), 0,	 1, -1,	vec2( 0,  0)} },
	{210, 	Cells{ 1,	vec2( 2,  1), vec2( 0,  1), vec2( 1,  1), 0,	-1,  1,	vec2( 2,  1)} },
	{240, 	Cells{-1,	vec2( 1,  2), vec2( 1,  0), vec2( 1,  1), 90,	 1,  1,	vec2( 1,  2)} },
	{240, 	Cells{ 1,	vec2( 1,  2), vec2( 0,  1), vec2( 0,  2), 90,	-1, -1,	vec2( 0,  0)} },
	{300, 	Cells{-1,	vec2(-1,  2), vec2( 0,  1), vec2( 0,  2), 270,	-1,  1,	vec2( 0,  0)} },
	{300, 	Cells{ 1,	vec2(-1,  2), vec2(-1,  0), vec2(-1,  1), 90,	-1,  1,	vec2(-1,  2)} },
	{330, 	Cells{-1,	vec2(-2,  1), vec2( 0,  1), vec2(-1,  1), 0,	-1, -1,	vec2(-2,  1)} },
	{330, 	Cells{ 1,	vec2(-2,  1), vec2(-1,  0), vec2(-2,  0), 0,	 1,  1,	vec2( 0,  0)} },
};

//const multimap<float, Cells> setPositionsByAngleL
//{
//	{30, 	Cells{-1,	vec2(-2, -1), vec2( 0, -1), vec2(-1, -1), 0,	-1,  1,	vec2( 0,  0)} },
//	{30, 	Cells{ 1,	vec2(-2, -1), vec2(-1,  0), vec2(-2,  0), 0,	 1, -1,	vec2(-2, -1)} },
//	{60, 	Cells{-1,	vec2(-1, -2), vec2( 0, -1), vec2( 0, -2), 270,	 1,  1,	vec2(-1, -2)} },
//	{60, 	Cells{ 1,	vec2(-1, -2), vec2(-1,  0), vec2(-1, -1), 90,	 1,  1,	vec2( 0,  0)} },
//	{120, 	Cells{-1,	vec2( 1, -2), vec2( 1,  0), vec2( 1, -1), 90,	-1,  1,	vec2( 0,  0)} },
//	{120, 	Cells{ 1,	vec2( 1, -2), vec2( 0, -1), vec2( 0, -2), 90,	 1, -1,	vec2( 1, -2)} },
//	{150, 	Cells{-1,	vec2( 2, -1), vec2( 1,  0), vec2( 2,  0), 0,	 1,  1,	vec2( 2, -1)} },
//	{150, 	Cells{ 1,	vec2( 2, -1), vec2( 0, -1), vec2( 1, -1), 0,	-1, -1,	vec2( 0,  0)} },
//	{210, 	Cells{-1,	vec2( 2,  1), vec2( 0,  1), vec2( 1,  1), 0,	 1, -1,	vec2( 0,  0)} },
//	{210, 	Cells{ 1,	vec2( 2,  1), vec2( 1,  0), vec2( 2,  0), 0,	-1,  1,	vec2( 2,  1)} },
//	{240, 	Cells{-1,	vec2( 1,  2), vec2( 0,  1), vec2( 0,  2), 90,	 1,  1,	vec2( 1,  2)} },
//	{240, 	Cells{ 1,	vec2( 1,  2), vec2( 1,  0), vec2( 1,  1), 90,	-1, -1,	vec2( 0,  0)} },
//	{300, 	Cells{-1,	vec2(-1,  2), vec2(-1,  0), vec2(-1,  1), 270,	-1,  1,	vec2( 0,  0)} },
//	{300, 	Cells{ 1,	vec2(-1,  2), vec2( 0,  1), vec2( 0,  2), 90,	-1,  1,	vec2(-1,  2)} },
//	{330, 	Cells{-1,	vec2(-2,  1), vec2(-1,  0), vec2(-2,  0), 0,	-1, -1,	vec2(-2,  1)} },
//	{330, 	Cells{ 1,	vec2(-2,  1), vec2( 0,  1), vec2(-1,  1), 0,	 1,  1,	vec2( 0,  0)} },
//};


const multimap<float, Cells> setPositionsByAngleS
{
	{30, 	Cells{ 0,	vec2(-2, -1), vec2(-1,  0), vec2(-1, -1), 0,	-1,  1,	vec2( 0,  0)} },
	{60, 	Cells{ 0,	vec2(-1, -2), vec2( 0, -1), vec2(-1, -1), 90,	 1,  1,	vec2( 0,  0)} },
	{120, 	Cells{ 0,	vec2( 1, -2), vec2( 0, -1), vec2( 1, -1), 90,	-1,  1,	vec2( 0,  0)} },
	{150, 	Cells{ 0,	vec2( 2, -1), vec2( 1,  0), vec2( 1, -1), 180,	 1,  1,	vec2( 0,  0)} },
	{210, 	Cells{ 0,	vec2( 2,  1), vec2( 1,  0), vec2( 1,  1), 180,	-1,  1,	vec2( 0,  0)} },
	{240, 	Cells{ 0,	vec2( 1,  2), vec2( 0,  1), vec2( 1,  1), 270,	 1,  1,	vec2( 0,  0)} },
	{300, 	Cells{ 0,	vec2(-1,  2), vec2( 0,  1), vec2(-1,  1), 270,	-1,  1,	vec2( 0,  0)} },
	{330, 	Cells{ 0,	vec2(-2,  1), vec2(-1,  0), vec2(-1,  1), 0,	 1,  1,	vec2( 0,  0)} },
};


/*
	h - only hammer
	e - only ellipsoid
	t - only tetramino
	f - ellipsoid n tetramino
*/
const map<string, string> animationsList
{
	{"fStand",	 "resources/3D/tetris/animations/stand.glb"		},
	{"fIdle0",	 "resources/3D/tetris/animations/idle0.glb"		},
	{"fIdle1",	 "resources/3D/tetris/animations/idle1.glb"		},
	{"eRun",	 "resources/3D/tetris/animations/run.glb"		},
	{"fJump0",	 "resources/3D/tetris/animations/jump0.glb"		},		
	{"fJump1",	 "resources/3D/tetris/animations/jump1.glb"		},
	{"hStand",	 "resources/3D/tetris/animations/standH.glb"	},
	{"hIdle0",	 "resources/3D/tetris/animations/idleH0.glb"	},
	{"hIdle1",	 "resources/3D/tetris/animations/idleH1.glb"	},
	{"hBlow",	 "resources/3D/tetris/animations/blow.glb"		},
};

#endif