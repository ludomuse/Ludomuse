#ifndef _CJSONPARSER_H_
#define _CJSONPARSER_H_

#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"

#include <string>
#include "CNode.h"
#include "CSceneNode.h"
#include "CEntityNode.h"
#include "CSpriteNode.h"
#include "CMenuNode.h"
#include "CLabelNode.h"

namespace LM
{

typedef const rapidjson::Value& RefJsonNode;


/// \brief Class responsible for parsing the json file and creating the game tre from it
class CJsonParser
{
private:
	rapidjson::Document m_oDocument;

 public:
  void BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename);

private:
	template <typename T>
	void ParseJson(RefJsonNode a_rJsonNode, T* a_pNode);
};


//////////////////////////////// templates specializations 


/// \brief the specialisation building entities in a entity
template <>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CSceneNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("entities"));
	RefJsonNode rEntities = a_rJsonNode["entities"];
	for (int i = 0; i < rEntities.Size(); ++i)
	{
		assert(rEntities[i]["object"].IsString());
		// the object type
		std::string sType = rEntities[i]["object"].GetString();
		// the object parameters
		RefJsonNode rParams = rEntities[i]["params"];
		if (sType == "Grid")
		{

		}
		else if (sType == "Image")
		{
			CSpriteNode* pSprite = new CSpriteNode(rParams["source"].GetString(),
				EAnchor::FLOAT, rParams["x"].GetInt(), rParams["y"].GetInt());
			a_pNode->AddChildNode(pSprite);
		}
		else if (sType == "Info")
		{
			std::string sFontName = rParams["font"].GetString();
			if (sFontName == "")
			{
				sFontName = "Default";
			}
			CLabelNode* pLabel = new CLabelNode(rParams["content"].GetString(),
				"", // TODO: FontName
				EAnchor(rParams["anchor"].GetInt())); // TODO check if constructor with index works
		}
		else if (sType == "Input")
		{

		}
		else if (sType == "Nav")
		{

		}
		else if (sType == "Reward")
		{

		}
		else if (sType == "Sound")
		{

		}
		else if (sType == "Text")
		{

		}
		else if (sType == "Video")
		{

		}
		//a_pNode->AddChildNode();
	}
}


/// \brief the specialisation building a scene
template <>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("content"));
	CSceneNode* pSceneNode = new CSceneNode();
	a_pNode->AddChildNode(pSceneNode);
	ParseJson(a_rJsonNode["content"], pSceneNode);
}


template <typename T>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode)
{
	// not supposed to be called

	//assert(a_rJsonNode.HasMember("screens"));
	//RefJsonNode rScenes = a_rJsonNode["screens"];
	//for (int i = 0; i < rScenes.Size(); ++i)
	//{
	//	ParseJson(rScenes[i], (CSceneNode*) a_pNode);
	//}
}



} // namespace LM

#endif /* _CJSONPARSER_H_ */
