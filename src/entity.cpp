#include "../include/entity.h"

Entity::Entity(vec3 pos, vec3 rot, vec3 scale)
{
	oPosition = pos;
	oRotation = rot;
	oScale = scale;
	
	hasCollision = 0;
	
	dPosition = vec3(0);
	dRotation = vec4(0);
	dScale = vec3(0);
}

void Entity::Move(vec3 pos)
{
	dPosition += pos;
}

void Entity::Rotate(vec3 rot)
{
	dRotation += rot;
}

void Entity::Scale(vec3 scale)
{
//	dScale += scale;
	oScale = scale;
}

void Entity::Reset()
{
	dPosition = vec3(0);
	dRotation = vec3(0);
//	dScale = vec3(0);
	oScale = vec3(1);
}

void Entity::ResetPosition()
{
	dPosition = vec3(0);
}

void Entity::ResetRotation()
{
	dRotation = vec3(0);
}

void Entity::ResetScale()
{
//	dScale = vec3(1);
	oScale = vec3(1);
}

void Entity::SetPosition(vec3 pos)
{
	
}

void Entity::SetAngle(vec3 angle)
{
	
}

void Entity::SetSize(vec3 size)
{
	
}

vec3 Entity::GetPosition()
{
	return (oPosition + dPosition) * 2.0f;
}

vec3 Entity::GetCellPosition()
{
	return oPosition + dPosition;
}

vec3 Entity::GetRotation()
{
	return oRotation + dRotation;
}

vec3 Entity::GetScale()
{
	return oScale /*+ dScale*/;
}