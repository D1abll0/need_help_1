#include "../include/model_manager.h"
#include "../include/model_animation.h"

MDLManager::MDLManager(MDLType type, const Shader& shader, const string& pathMDL, const map<string, string> animationsList)
{
	MDL = new Model(pathMDL, 0);
	if(type == MDLType::DYNAMIC)
	{
		for(auto animation : animationsList)
			animations.insert({animation.first, new Animation(animation.second)});
		animator = new Animator(animations.begin()->second);
	}
	
	this->type = type;
	this->shader = shader;	
}

void MDLManager::SetOrigin(vec3 pos_, vec3 rot_, vec3 scale_, vec2 uv_)
{
	pos = pos_;
	rot = rot_;
	scale = scale_;
	
	float aX = rot_.x;
	float aY = rot_.y;
	float aZ = rot_.z;
	uv = uv_;
	
	model = mat4(1);

	model = translate(model, pos_);

	model = rotate(model, radians(rot_.x), vec3(1, 0, 0));
	model = rotate(model, radians(rot_.y), vec3(0, 1, 0));
	model = rotate(model, radians(rot_.z), vec3(0, 0, 1));
	
	model = glm::scale(model, scale_);
}

void MDLManager::Move(vec3 dPos)
{
	model = glm::scale(model, vec3(1) / scale);
	model = translate(model, dPos);	
	model = glm::scale(model, scale);
	pos += dPos;
}

void MDLManager::SetViewPos(vec3 vPos)
{
	viewPos = vPos;
}

/*Light begin*/
void MDLManager::SetLight(LightCaster type, vec3 pos, vec3 dir)
{
	Light light;
	if(type == Direction) light.type[0] = 1.0f;
	if(type == Point) light.type[1] = 1.0f;
	if(type == Spot) light.type[2] = 1.0f;
	light.Position = pos;
	light.Direction = dir;
		
	lights.push_back(light);
}

void MDLManager::ClearLights()
{
	lights.clear();
}

void MDLManager::SetAmbient(vec3 color)
{
	lights[lights.size()].ambient = color;
}

void MDLManager::SetAmbient(float r, float g, float b)
{
	lights[lights.size()].ambient = vec3(r, g, b);
}

void MDLManager::SetSpecular(vec3 color)
{
	lights[lights.size()].specular = color;
}

void MDLManager::SetSpecular(float r, float g, float b)
{
	lights[lights.size()].specular = vec3(r, g, b);
}

void MDLManager::SetDiffuse(vec3 color)
{
	lights[lights.size()].diffuse = color;
}

void MDLManager::SetDiffuse(float r, float g, float b)
{
	lights[lights.size()].diffuse = vec3(r, g, b);
}

void MDLManager::SetLightingColor(vec3 color)
{
	lights[lights.size()].color = color;
}

void MDLManager::SetLightingColor(float r, float g, float b)
{
	lights[lights.size()].color = vec3(r, g, b);
}
/*end*/

/*Material begin*/
void MDLManager::SetMaterialColor(float time)
{
//	time = Time::dt * 100000;
	int index = int(time * 1000) % (sizeof(Colors) / sizeof(vec3));
	
	if(material.color == vec3(1))
		material.color = Colors[index];
	
	if(material.textureType == -1) 
		material.textureType = (int(time * 1000) % 2) + 1;
}

void MDLManager::SetMaterialColor(Color color)
{
	material.color = Colors[color];
}

void MDLManager::SetMaterialColor(int index)
{
	material.color = Colors[index];
}

void MDLManager::SetTextureType(int index)
{
	if(material.textureType == -1)
		material.textureType = index;
}

void MDLManager::SetShininess(float value)
{
	material.shininess = value;
}
/*end*/

void MDLManager::SetShadow(const Shader& shader)
{
	ShadowShader = shader;
	hasShadow = 1;
//	shadow = new Shadow(shader, mapSize);
}

/*Matrix begin*/
void MDLManager::SetModelMat(mat4 mat)
{
	model = mat;
}

void MDLManager::SetViewMat(mat4 mat)
{
	view = mat;
}

void MDLManager::SetProjMat(mat4 mat)
{
	projection = mat;
}

void MDLManager::AddTexture(unsigned int id, string type)
{
	for(int i = 0; i < MDL->meshes.size(); i++)
		MDL->meshes[i].textures.push_back({id, type, 0, ""});
}

mat4 MDLManager::GetModelMat()
{
	return model;
}

mat4 MDLManager::GetViewMat()
{
	return view;
}

mat4 MDLManager::GetProjMat()
{
	return projection;
}

void MDLManager::SetTexSize(float size, vec2 uv)
{
	this->size = size;
	this->uv = uv;
}

void MDLManager::Scale(vec3 scale_)
{
	model = glm::scale(model/*mat4(1.0)*/, scale_);
}

void MDLManager::Scale(float scale_)
{
	model = glm::scale(model/*mat4(1.0)*/, vec3(scale_, scale_, scale_));
}

void MDLManager::Translate(vec3 pos)
{
	model = translate(model/*mat4(1.0)*/, pos);
}

void MDLManager::Translate(float pos)
{
	model = translate(model/*mat4(1.0)*/, vec3(pos, pos, pos));	
}

void MDLManager::Rotate(float angle, vec3 axes)
{
//	model = rotate(model/*mat4(1.0)*/, radians(angle), axes);
	mat4 matT = rotate(mat4(1.0), radians(angle), axes);
	model = matT * model;
}

void MDLManager::Rotate(float angle, float axes)
{
	model = rotate(model/*mat4(1.0)*/, radians(angle), vec3(axes, axes, axes));	
}
/*end*/

void MDLManager::SetLightMatrix(mat4 mat)
{
	lightSpace = mat;
}


void MDLManager::Draw()
{
	shader.use();
	
	if(type == MDLType::DYNAMIC)
	{
		auto bones = animator->GetFinalBoneMatrices();
		for(int i = 0; i < bones.size(); i++)
			shader.setMat4("finalBonesMatrices[" + to_string(i) + "]", bones[i]);		
	}

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setFloat("size", size);
	shader.setVec2("uv", uv);
	
	shader.setVec3("viewPos", viewPos);
	
	shader.setVec3("material.color", material.color);
	shader.setFloat("material.shininess", material.shininess);
	
	for(int i = 0; i < lights.size(); i++)
	{
		shader.setVec3("light["+to_string(i)+"].position", lights[i].Position);
		shader.setVec3("light["+to_string(i)+"].direction", lights[i].Direction);
		shader.setVec3("light["+to_string(i)+"].ambient", lights[i].ambient);
		shader.setVec3("light["+to_string(i)+"].diffuse", lights[i].diffuse);
		shader.setVec3("light["+to_string(i)+"].specular", lights[i].specular);
		
		shader.setFloat("light["+to_string(i)+"].cutOff", cos(radians(1.)));
		shader.setFloat("light["+to_string(i)+"].outerCutOff", cos(radians(lights[i].cutOff)));
		
		shader.setFloat("light["+to_string(i)+"].constant", lights[i].constant);
		shader.setFloat("light["+to_string(i)+"].linear", lights[i].linear);
		shader.setFloat("light["+to_string(i)+"].quadratic", lights[i].quadratic);	
		
		shader.setVec3("light["+to_string(i)+"].type", lights[i].type);
	}
	
	MDL->Draw(shader, material.textureType);
	shader.stop();
}

void MDLManager::AssemblyShadow()
{
	Shader shader = Shadow::GetShader();
	shader.use();
	shader.setMat4("model", model);
	shader.stop();
	Render(shader);
}

void MDLManager::Render(Shader& shader)
{
	shader.use();
	MDL->Draw(shader, material.textureType);
	shader.stop();
}

void MDLManager::Animate(float dt)
{
	animator->UpdateAnimation(dt);
}

void MDLManager::PlayAnimation(const string& animName)
{
//	if(animations.find(animName) != animations.end())
//	{
//		animator->PlayAnimation(animations[animName]);
//	}
}



