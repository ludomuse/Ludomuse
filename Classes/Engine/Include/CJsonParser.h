#ifndef _CJSONPARSER_H_
#define _CJSONPARSER_H_

#include "../../cocos2d/external/json/rapidjson.h"
#include "../../cocos2d/external/json/document.h"

#include <string>
#include "CNode.h"
#include "CSceneNode.h"
#include "CEntityNode.h"

namespace LM
{

	typedef const rapidjson::Value& RefJsonNode;

class CJsonParser
{
private:
	rapidjson::Document m_oDocument;

 public:
  void BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename);

private:
	template <typename T>
	void ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode);
};




//////////////////////////////// templates specializations 


template <>
void CJsonParser::ParseJson<CNode>(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember["screens"]);
	RefJsonNode rScenes = a_rJsonNode["screens"];
	for (int i = 0; i < rScenes.Size(); ++i)
	{
		ParseJson<CSceneNode>(rScenes[i], a_pNode);
	}
}


template <>
void CJsonParser::ParseJson<CSceneNode>(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember["content"]);
	CSceneNode* pSceneNode = new CSceneNode();
	a_pNode->AddChildNode(pSceneNode);
	//ParseJson<CEntityNode>(a_rJsonNode["content"], pSceneNode);
}

//template <>
//void CJsonParser::ParseJson<CEntityNode>(RefJsonNode a_rJsonNode, CNode* a_pNode)
//{
//
//}



} // namespace LM

#endif /* _CJSONPARSER_H_ */
