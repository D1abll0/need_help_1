#version 330 core
layout (location = 0) in vec3 aPos;

#define NUM_OF_LIGHTS 3
uniform mat4 lightSpaceMatrix[NUM_OF_LIGHTS];
uniform mat4 model;
uniform mat4 MVP;

struct Light
{
	vec3 position;
	vec3 color;
	mat4 lightSpaceMatrix;
};

uniform Light lights[NUM_OF_LIGHTS];

void main()
{
	gl_Position = lightSpaceMatrix[0] * model * vec4(aPos, 1.0);
//	gl_Position = MVP * vec4(aPos, 1.0);
}