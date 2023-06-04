#ifndef SHADOW_H
#define SHADOW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

#include <vector>

#include "shader.h"
#include "model_animation.h"

using namespace std;
using namespace glm;

class Shadow
{
public:	

	static unsigned int mID;
	unsigned int FBO;
	int mSize;
	float near, far;
	static Shader shader;
	static mat4 LightSpaceMatrix;
	
	
	Shadow() = default;
	Shadow(const Shader& shader, unsigned int size);
	
	void GenIDs();
	void CreateDepthMap();
	void SetDepthMapSize(unsigned int size);
	void SetLightMatrix(vec3 lightsPos);
	void Disable();
	
	static Shader& GetShader();
	static mat4 GetLightSpaceMatrix();
	static const unsigned int& GetId();
};

#endif