#ifndef LIGHT_H
#define LIGHT_H

#include "constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Light
{
	vec3 Position;
	vec3 Direction;
	
	vec3 ambient = vec3(.2, .2, .2);
	vec3 diffuse = vec3(.5, .5, .5);
	vec3 specular = vec3(.5, .5, .5);
	
	float cutOff = 15.f;
	
    float constant = 1.f;
    float linear = 0.09f;
    float quadratic = 0.032f;
	
	vec3 color = vec3(1.);
	vec3 type = vec3(.0);
};

#endif