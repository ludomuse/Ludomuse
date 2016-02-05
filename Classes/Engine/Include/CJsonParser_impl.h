//////////////////////////////// templates specializations 

#include "CCallback.h"
#include "CEntityNode.h"

/// \brief the specialisation building entities in a entity
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CSceneNode* a_pSceneNode)
{

	assert(a_rJsonNode["type"].IsString());
	// the object type
	std::string sType = a_rJsonNode["type"].GetString();
	// the object parameters
	RefJsonNode rParams = a_rJsonNode["params"];

	int x, y, width, height;
	if (rParams.HasMember("x")) 
	{
		x = rParams["x"].GetInt();
		y = rParams["y"].GetInt();
	}
	else
	{
		x = y = 0;
	}
	if (rParams.HasMember("width")) 
	{
		width = rParams["width"].GetInt();
		height = rParams["height"].GetInt();
	}
	else
	{
		width = height = 0;
	}

	CEntityNode* pEntity(nullptr);

	if (sType == "Grid")
	{

	}

	else if (sType == "Grid2")
	{
		CCLOG("Grid construction");
           pEntity = new CMenuNode(
               rParams["normal"].GetString(),
               rParams["selected"].GetString(),
               CCallback(m_pKernel,
				   std::string(rParams["action"].GetString()) == "connect" ?
                         &CKernel::Connect : &CKernel::SendMessage),
               IntToAnchor(rParams["anchor"].GetInt()),
               width,
               height,
               x,
               y);
		   CCLOG("grid constructed");
        }

	else if (sType == "Image")
	{
		pEntity = new CSpriteNode(rParams["source"].GetString(),
			IntToAnchor(rParams["anchor"].GetInt()),
			width, height,
			x, y);
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
			width,
			height,
			x,
			y);

		if (rParams.HasMember("content"))
		{
			CLabelNode* pText = new CLabelNode(
				rParams["content"].GetString(),
				"fonts/arial.ttf",
				24,
				IntToAnchor(rParams["anchor"].GetInt()),
				//EAnchor::FLOAT,
				width,
				height,
				0,
				0);

			pEntity->AddChildNode(pText);
		}
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
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);

		if (rParams["cache"]["visible"].GetBool())
		{
			CSpriteNode* pSpriteNode = new CSpriteNode(
				rParams["cache"]["backgroundImage"].GetString(),
				IntToAnchor(rParams["anchor"].GetInt()),
				width,
				height,
				x,
				y);

			pEntity->AddChildNode(pSpriteNode);
		}

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
	CSceneNode* pSceneNode = new CSceneNode(a_rJsonNode["scene"].GetString());
	a_pNode->AddChildNode(pSceneNode);
	if (a_rJsonNode.HasMember("content"))
	{
		RefJsonNode rEntities = a_rJsonNode["content"];
		for (int i = 0; i < rEntities.Size(); ++i)
		{
			ParseJson(rEntities[i], pSceneNode);
		}
	}
	if (a_rJsonNode.HasMember("navigation"))
	{
		RefJsonNode rNavItems = a_rJsonNode["navigation"];
		for (int i = 0; i < rNavItems.Size(); ++i)
		{
			ParseJson(rNavItems[i], pSceneNode);
		}
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

