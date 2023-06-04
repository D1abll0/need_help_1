#ifndef FIGURE_H
#define FIGURE_H

#include "camera.h"
#include "entity.h"
#include "world.h"


class Figure : public Entity
{
	
public:	

	int ID;
	MDLManager MDL;
	int type;
	
	vec3 position;
	vec3 scale;
	float angleY = 0;
	float angleX = 0;
	float angleZ = 0;
	bool isDropped = 0;
	bool isTetramino = 0;
	bool isVisible = 1;
	
	float color = 0;
	int textureType = 0;
	bool isNoModel = 0;
	
	Figure() = default;
//	Figure();
	Figure(const MDLManager& MDL, vec3 position, float angle, vec3 scale, int type = -1);
	
	int GetType();
	void RandomTexture();
	void SetColorTexture(float color, int texture);
	void DroppedAnimate();
	
	void RenderShadow(Camera& camera);
	void Draw(Camera& camera);
};

#endif