#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "model_animation.h"
#include "animator.h"
#include "shader.h"
#include "light.h"
#include "shadow.h"
#include "constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\dual_quaternion.hpp>

#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace glm;

#define NUM_OF_LIGHTS 2

struct Material
{
	float shininess = 32.f;
	vec3 color = vec3(1.);
	int textureType = -1;
};

class MDLManager
{
public:
	Model* MDL;
	map<string, Animation*> animations;
	Animator* animator;
	MDLType type = MDLType::STATIC;
	Shader shader;
	vector<Light> lights;
	Shader ShadowShader;
	Shadow SHADOW;
	Material material;
	
	mat4 model = mat4(1.0f);
	mat4 view = mat4(1.0f);
	mat4 projection = mat4(1.0f);
	mat4 lightSpace;
	
	vec3 viewPos;
	
	float size = 1.0;
	vec2 uv = vec2(1);
	int lightsCount = 0;	
	bool shadowOffOn = 1;
	bool hasShadow = 0;

	unsigned int opacity = 100;
	
	vec3 pos, rot, scale;
	
	MDLManager() = default;
	MDLManager(MDLType type, const Shader& shader, const string& pathMDL, const map<string, string> animationsList = {});

	void SetOrigin(vec3 pos_, vec3 rot_, vec3 scale_, vec2 uv_ = vec2(1));
	
	void SetModelMat(mat4 mat);
	void SetViewMat(mat4 mat);
	void SetProjMat(mat4 mat);
	
	void SetViewPos(vec3 vPos);
	
	void SetLight(LightCaster type, vec3 pos, vec3 dir = vec3(1.));
	void ClearLights();
	void SetAmbient(vec3 color);
	void SetAmbient(float r, float g, float b);
	void SetSpecular(vec3 color);
	void SetSpecular(float r, float g, float b);
	void SetDiffuse(vec3 color);
	void SetDiffuse(float r, float g, float b);
	void SetLightingColor(vec3 color);
	void SetLightingColor(float r, float g, float b);
	
	void SetMaterialColor(float time);
	void SetMaterialColor(Color color);
	void SetMaterialColor(int index);
	void SetTextureType(int index);
	void SetShininess(float value);
		
	void SetTexSize(float size, vec2 uv);
	
	void SetShadow(const Shader& shader);
	void SetLightMatrix(mat4 mat);
	
	void AddTexture(unsigned int id, string type);
	
	mat4 GetModelMat();
	mat4 GetViewMat();
	mat4 GetProjMat();
	
	void Scale(vec3 scale_);
	void Scale(float scale_);
	void Translate(vec3 pos);
	void Translate(float pos);
	void Rotate(float angle, vec3 axes);
	void Rotate(float angle, float axes);
	
	void Move(vec3 dPos);
	
	void Animate(float dt);
	void PlayAnimation(const string& animName);
	
	void Draw();
	void AssemblyShadow();
	void Render(Shader& shader);
};
#endif