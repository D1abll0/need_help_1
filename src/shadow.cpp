#include "../include/shadow.h"

mat4 Shadow::LightSpaceMatrix = mat4(0);
Shader Shadow::shader;
unsigned int Shadow::mID = 0;

Shadow::Shadow(const Shader& shader, unsigned int size)
{
	this->shader = shader;
	mSize = size;
	near = 1.f;
	far = 100.f;
	
	GenIDs();
	CreateDepthMap();
}

void Shadow::GenIDs()
{
	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &mID);	
}

void Shadow::CreateDepthMap()
{
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mSize, mSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void Shadow::SetDepthMapSize(unsigned int size)
{
	mSize = size;
	CreateDepthMap();
}

void Shadow::SetLightMatrix(vec3 lightsPos)
{
	mat4 projection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);
	mat4 view = lookAt(lightsPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
	LightSpaceMatrix = projection * view;
		
	shader.use();
	shader.setMat4("lightSpaceMatrix", LightSpaceMatrix);
	
	glViewport(0, 0, mSize, mSize);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::Disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glViewport(0, 0, 800, 600);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Shader& Shadow::GetShader()
{
	return shader;
}

mat4 Shadow::GetLightSpaceMatrix()
{
	return LightSpaceMatrix;
}

const unsigned int& Shadow::GetId()
{
	return mID;
}




