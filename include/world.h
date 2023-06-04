#ifndef WORLD_H
#define WORLD_H

#include "constants.h"
#include "skybox.h"
#include "camera.h"
#include "model_manager.h"
#include "shadow.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;
using namespace glm;

//#define GRAVITY 0.001

struct Field
{
	int sizeH;
	int sizeW;
	vector<Cell> field;
	
	Field() = default;
	Field(int sizeH, int sizeW);
	
	bool isCellEmpty(int x, int z);
	bool canMove(int x, int z);
	bool AssemblyCells(TETRAMINO_TYPE figure, vec2 origin, float angle, int da, Cells& c);
	int canSetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da);
	bool SetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da);
	
	
	void print();
};

class World
{
	int height;
	static int fieldSize;
	static int curLevel;
	
	vector<Field> fields;
	vector<TETRAMINO_TYPE>fieldOfDropped;
	
	SkyBox* SKY = nullptr;
	MDLManager* FLOOR = nullptr;
	MDLManager* ZONE = nullptr;
	vec3 floorPos;
	
public:
	static const float GRAVITY;
		
	World() = default;
	World(int fieldSize, int height = 10);
	
	bool AddDroppedTetramino(int x, int z, int index);
	bool SetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da);
	static int GetSize();
	int GetHeight();
	static int GetLevel();
	void AddFloor(MDLManager* FLOOR, MDLManager* ZONE);
	void AddSky(SkyBox* SKY);
	
	void Draw(Camera& camera);
	
	void print();
};

bool isAbroad(int x, int z, int w, int h);

#endif