#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "bone.h"
#include <functional>
#include "animdata.h"
#include "model_animation.h"
#include "time.h"

using namespace std;
using namespace glm;

struct AssimpNodeData
{
	mat4 transformation;
	string name;
	int childrenCount;
	vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;

	Animation(const string& animationPath);

	~Animation(){}

	Bone* FindBone(const string& name);
	
	inline float GetTicksPerSecond() { return m_TicksPerSecond; }
	inline float GetDuration() { return m_Duration;}
	inline float GetStartTime() { return startTime; }
	inline bool IsFinished()
	{
		return isFinished;
	}
	inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
	inline const map<string,BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }
	void CollectBoneInfo(aiNode* node, const aiScene* scene);
	
	void RestartAnimation(bool state);
	void SetStartTime();
	void Finish();

private:
	void ReadMissingBones(const aiAnimation* animation);

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
	float m_Duration;
	int m_TicksPerSecond;
	float startTime;
	bool isFinished;
	
	vector<Bone> m_Bones;
	vector<aiBone*> bones;
	AssimpNodeData m_RootNode;
	map<string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;
};

#endif