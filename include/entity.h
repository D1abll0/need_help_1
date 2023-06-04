#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "model_manager.h"


using namespace glm;

class Entity
{
public:	
	MDLManager MDL;
	
	vec3 oPosition = vec3(0);
	vec3 oRotation = vec3(0);
	vec3 oScale = vec3(0);
	
	vec3 dPosition;
	vec3 dRotation;
	vec3 dScale;
	
	bool hasCollision;
	
	
	Entity() = default;
	Entity(vec3 pos, vec3 rot, vec3 scale);
	
	void Move(vec3 pos);
	void Rotate(vec3 rot);
	void Scale(vec3 scale);
	
	void Reset();
	void ResetPosition();
	void ResetRotation();
	void ResetScale();
	
	void SetPosition(vec3 pos);
	void SetAngle(vec3 angle);
	void SetSize(vec3 size);
	
	vec3 GetPosition();
	vec3 GetCellPosition();
	vec3 GetRotation();
	vec3 GetScale();
	
	void AddShadow();
};

#endif