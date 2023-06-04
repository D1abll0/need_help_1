#include "../include/world.h"

/*field*/


Field::Field(int sizeH, int sizeW)
{
	this->sizeH = sizeH;
	this->sizeW = sizeW;
	field.resize(sizeH * sizeW, {TETRAMINO_TYPE::NONE, 0});	
}

bool Field::isCellEmpty(int x, int z)
{
	if(isAbroad(x, z, sizeW, sizeH)) return 0;
	if(field[x + sizeW * z].tetraminoPart == TETRAMINO_TYPE::NONE) return 1;
	return 0;
}

bool Field::canMove(int x, int z)
{
	if(isAbroad(x, z, sizeW, sizeH)) return 0;
	if(field[x + sizeW * z].isBusy) return 0;
	return 1;	
}

bool Field::AssemblyCells(TETRAMINO_TYPE figure, vec2 origin, float angle, int da, Cells& c)
{
		multimap<float, Cells> cellsPositions;
		
		if(figure == TETRAMINO_TYPE::I) cellsPositions = setPositionsByAngleI;
		if(figure == TETRAMINO_TYPE::O) cellsPositions = setPositionsByAngleO;
		if(figure == TETRAMINO_TYPE::T) cellsPositions = setPositionsByAngleT;
		if(figure == TETRAMINO_TYPE::S) cellsPositions = setPositionsByAngleS;
		if(figure == TETRAMINO_TYPE::L) cellsPositions = setPositionsByAngleL;
		
		int i = 0;
		for(auto cell : cellsPositions)
		{
			i++;
			if(cell.first == angle && cell.second.da == da)
			{
				c = cell.second;
				break;
			}
			
			if(i == cellsPositions.size()) return 0;
		}
		
		c.c1 += origin;
		c.c2 += origin;
		c.c3 += origin;	
				
		return 1;	
}

int Field::canSetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da)
{
	Cells c;
	
	if(!AssemblyCells(figure, origin, angle, da, c)) return 0;
	if(!isAbroad(c.c1[1], c.c1[0], sizeW, sizeH) && !isAbroad(c.c2[1], c.c2[0], sizeW, sizeH) && !isAbroad(c.c3[1], c.c3[0], sizeW, sizeH))
	{
		if(isCellEmpty(origin[1], origin[0]) && isCellEmpty(c.c1[1], c.c1[0]) && isCellEmpty(c.c2[1], c.c2[0]) && isCellEmpty(c.c3[1], c.c3[0]))
			return 1;
		else
			return -1;
	}
	return 0;	
}

bool Field::SetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da)
{
	Cells c;
	
	if(!AssemblyCells(figure, origin, angle, da, c)) return 0;
	
	field[origin[1] + sizeW * origin[0]].tetraminoPart = figure;
	field[c.c1[1] + sizeW * c.c1[0]].tetraminoPart = figure;
	field[c.c2[1] + sizeW * c.c2[0]].tetraminoPart = figure;
	field[c.c3[1] + sizeW * c.c3[0]].tetraminoPart = figure;
	
	return 1;	
}



void Field::print()
{
	for(int i = 0; i < sizeW; i++)
	{
		for(int j = 0; j < sizeH; j++)
		{
			cout << (int)field[j + sizeW * i].tetraminoPart << " ";
		}
		cout << endl;
	}
}


/*world*/
const float World::GRAVITY = 9.8;
int World::curLevel = 0;
int World::fieldSize = 0; 


World::World(int fieldSize, int height)
{
	this->fieldSize = fieldSize;	
	this->height = height;
	fields.resize(height, {Field(fieldSize, fieldSize)});
	fieldOfDropped.resize(fieldSize * fieldSize, {TETRAMINO_TYPE::NONE});
	
	curLevel = 1;
}

bool World::AddDroppedTetramino(int x, int z, int index)
{
	if(isAbroad(x, z, fieldSize, fieldSize)) return 0;
	if(fieldOfDropped[x + fieldSize * z] != TETRAMINO_TYPE::NONE) return 0;
	
	fieldOfDropped[x + fieldSize * z] = TETRAMINO_TYPE(index + 1);
	
	return 1;
}

bool World::SetFigure(TETRAMINO_TYPE figure, vec2 origin, float angle, int da)
{
	int canSet = fields[curLevel].canSetFigure(figure, vec2(origin[1], origin[0]), angle, da);
	if(canSet != 0)
	{
//		if(canSet == -1)curLevel++;

		if(fields[curLevel].SetFigure(figure, vec2(origin[1], origin[0]), angle, da))
		{
			return 1;
		}
	}
	
	return 0;	
}

int World::GetSize()
{
	return fieldSize;
}

int World::GetHeight()
{
	return height;
}

int World::GetLevel()
{
	return curLevel;
}

void World::AddFloor(MDLManager* FLOOR, MDLManager* ZONE)
{
	this->FLOOR = FLOOR;
	this->ZONE = ZONE;
}

void World::AddSky(SkyBox* SKY)
{
	this->SKY = SKY;
}

void World::Draw(Camera& camera)
{
	vec3 lightPos(.0f, 2.0f, -1.0f);
	
	if(FLOOR != nullptr)
	{
		cout << "";
		FLOOR->SetOrigin(vec3(fieldSize-1, curLevel-1+0.01f, fieldSize-1), vec3(0), vec3(fieldSize, 1, fieldSize), vec2(fieldSize));		
		FLOOR->SetViewMat(camera.GetViewMatrix());
		FLOOR->SetProjMat(camera.GetProjectionMatrix());
		FLOOR->ClearLights();
		FLOOR->SetViewPos(camera.Position);
		FLOOR->SetLightMatrix(Shadow::GetLightSpaceMatrix());
		FLOOR->SetLight(Direction, vec3(0), lightPos * -1.0f);
		FLOOR->Draw();	
	}
	
	if(ZONE != nullptr)
	{
		ZONE->SetOrigin(vec3(fieldSize-1, float(curLevel)+1.01f, fieldSize-1), vec3(0), vec3(fieldSize, 1, fieldSize), vec2(fieldSize));
		ZONE->SetViewMat(camera.GetViewMatrix());
		ZONE->SetProjMat(camera.GetProjectionMatrix());
		ZONE->ClearLights();
		ZONE->SetLightMatrix(Shadow::GetLightSpaceMatrix());
		ZONE->SetViewPos(camera.Position);
		ZONE->SetLight(Direction, vec3(0), lightPos * -1.0f);
		ZONE->Draw();	
	}	
	
	if(SKY != nullptr)
	{	
		SKY->SetViewMat(camera.GetViewMatrix());
		SKY->SetProjMat(camera.GetProjectionMatrix());
		SKY->Draw();
	}
}



void World::print()
{
	fields[curLevel].print();
}



/*functions*/

bool isAbroad(int x, int z, int w, int h)
{
	if(x < 0 || x >= w || z < 0 || z >= h) return 1;
	return 0;	
}




