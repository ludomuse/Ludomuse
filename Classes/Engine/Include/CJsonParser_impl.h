//////////////////////////////// templates specializations 

#include "CCallback.h"

/// \brief the specialisation building entities in a entity
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CSceneNode* a_pSceneNode)
{

	assert(a_rJsonNode["object"].IsString());
	// the object type
	std::string sType = a_rJsonNode["object"].GetString();
	// the object parameters
	RefJsonNode rParams = a_rJsonNode["params"];

	CNode* pEntity(nullptr);

	if (sType == "Grid")
	{

	}

	else if (sType == "Image")
	{
		pEntity = new CSpriteNode(rParams["source"].GetString(),
			IntToAnchor(rParams["anchor"].GetInt()), 
			rParams["x"].GetInt(), rParams["y"].GetInt());
	}


	else if (sType == "Info")
	{

	}


	else if (sType == "Input")
	{

	}


	else if (sType == "Nav")
	{
		// TODO
		//auto lambda = [this](cocos2d::Ref* pSender)
		//{
		//	m_pKernel->NavNext(pSender);
		//	//CCLOG("HI");
		//};
		pEntity = new CMenuNode(
			rParams["normal"].GetString(),
			rParams["selected"].GetString(),
			CCallback(m_pKernel, 
				(std::string(rParams["action"].GetString()) == "next") ?
								 &CKernel::NavNext : &CKernel::NavPrevious),
			IntToAnchor(rParams["anchor"].GetInt()),
			rParams["x"].GetInt(),
			rParams["y"].GetInt());
	}


	else if (sType == "Reward")
	{

	}


	else if (sType == "Sound")
	{

	}


	else if (sType == "Text")
	{
		std::string sFontName = "fonts/arial.ttf";
		if (rParams.HasMember("font"))
		{
			std::string sFontName = rParams["font"].GetString();
		}
		if (sFontName == "")
		{
			sFontName = "fonts/arial.ttf";
		}
		int iFontSize = rParams["fontSize"].GetInt();
		if (iFontSize < 1)
		{
			iFontSize = 24;
		}
		pEntity = new CLabelNode(rParams["content"].GetString(),
			sFontName, 
			iFontSize,
			IntToAnchor(rParams["anchor"].GetInt()));

	}


	else if (sType == "Video")
	{

	}

	if (pEntity != nullptr) {
		a_pSceneNode->AddChildNode(pEntity);
	}

}


/// \brief the specialisation building a scene
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("entities"));
	CSceneNode* pSceneNode = new CSceneNode();
	a_pNode->AddChildNode(pSceneNode);
	RefJsonNode rEntities = a_rJsonNode["entities"];
	for (int i = 0; i < rEntities.Size(); ++i)
	{
		ParseJson(rEntities[i], pSceneNode);
	}
}


template <typename T>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode)
{
	// not supposed to be called

	//assert(a_rJsonNode.HasMember("screens"));
	//RefJsonNode rScenes = a_rJsonNode["screens"];
	//for (int i = 0; i < rScenes.Size(); ++i)
	//{
	//	ParseJson(rScenes[i], dynamic_cast<CSceneNode*>(a_pNode));
	//}
}

