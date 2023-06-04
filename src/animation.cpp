#include "../include/animation.h"

float max_(float a, float b)
{
	return a < b ? b : a;
}

Animation::Animation(const string& animationPath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHierarchyData(m_RootNode, scene->mRootNode);
	CollectBoneInfo(scene->mRootNode, scene);
	ReadMissingBones(animation);
}

Bone* Animation::FindBone(const string& name)
{
	auto iter = find_if(m_Bones.begin(), m_Bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == m_Bones.end()) return nullptr;
	else return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation)
{
	int size = animation->mNumChannels;

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		string boneName = channel->mNodeName.data;

		if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
		{
			m_BoneInfoMap[boneName].id = m_BoneCounter;
			m_BoneCounter++;
		}
		m_Bones.push_back(Bone(channel->mNodeName.data, m_BoneInfoMap[channel->mNodeName.data].id, channel));
	}
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

void Animation::CollectBoneInfo(aiNode* node, const aiScene* scene)
{
    if (node->mNumMeshes > 0 && node->mMeshes[0] >= 0)
	{
        aiMesh* mesh = scene->mMeshes[node->mMeshes[0]];
        for (unsigned int j = 0; j < mesh->mNumBones; j++)
		{
            aiBone* bone = mesh->mBones[j];
            bones.push_back(bone);
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
		CollectBoneInfo(node->mChildren[i], scene);
		
	for(int boneIndex = 0; boneIndex < bones.size(); boneIndex++)
	{
		int boneID = -1;
		string boneName = bones[boneIndex]->mName.C_Str();
		if(m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_BoneCounter;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(bones[boneIndex]->mOffsetMatrix);
			m_BoneInfoMap[boneName] = newBoneInfo;
			boneID = m_BoneCounter;
			m_BoneCounter++;			
		}
		else
		{
			boneID = m_BoneInfoMap[boneName].id;
		}
	} 		
}

void Animation::RestartAnimation(bool state)
{
	isFinished = 0;
}

void Animation::SetStartTime()
{
	startTime = Time::cTime;
	isFinished = 0;
}

void Animation::Finish()
{
	isFinished = 1;
}



