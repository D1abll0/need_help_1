#include "../include/figure.h"

Figure::Figure(const MDLManager& MDL, vec3 position, float angle, vec3 scale, int type)
{
	this->MDL = MDL;
	this->position = position;	
	this->angleY = angle;
	if(scale.x == 0) scale.x = 1;
	if(scale.y == 0) scale.y = 1;
	if(scale.z == 0) scale.z = 1;
	this->scale = scale;	
	this->type = type + 1;
	isVisible = 1;
	
	this->MDL.AddTexture(Shadow::GetId(), "shadowMap");
	
	RandomTexture();
}

//Figure::Figure()
//{
//	isNoModel = 1;
//}

int Figure::GetType()
{
	return type;
}

void Figure::RandomTexture()
{
	color = Time::cTime;
	textureType = int(Time::cTime) % 2 + 1;
	
	MDL.SetMaterialColor(color);
	MDL.SetTextureType(textureType);		
}

void Figure::SetColorTexture(float color, int texture)
{
	this->color = color;
	this->textureType = texture;
	
	MDL.SetMaterialColor(color);
	MDL.SetTextureType(textureType);	
}

void Figure::DroppedAnimate()
{
	scale = vec3(0.2);
	angleX = 90.f;
	angleZ += 1.0f;
	
	position.y = World::GetLevel() + 0.5 + 1 * abs(sin(Time::cTime*4)/2);
}

void Figure::RenderShadow(Camera& camera)
{
	if(isDropped)DroppedAnimate();
	
	Reset();
	Move(position);
	Rotate(vec3(angleX, angleY, angleZ));
	Scale(scale);
	MDL.SetOrigin(GetPosition(), GetRotation(), GetScale());
	
	MDL.AssemblyShadow();
}

void Figure::Draw(Camera& camera)
{
	if(isDropped)DroppedAnimate();
	
	Reset();
	Move(position);
	Rotate(vec3(angleX, angleY, angleZ));
	Scale(scale);
	vec3 lightPos(.0f, 2.0f, -1.0f);
	
	MDL.SetOrigin(GetPosition(), GetRotation(), GetScale());
	MDL.SetViewMat(camera.GetViewMatrix());
	MDL.SetProjMat(camera.GetProjectionMatrix());
	MDL.ClearLights();
	MDL.SetLight(Direction, vec3(0), lightPos * -1.0f);
	MDL.SetViewPos(camera.Position);
	MDL.Draw();	
}