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
	/// \brief the currently processed json document
	rapidjson::Document m_oDocument;

 public:
	 /// \brief Build the tree of root a_pRoot from a_sFilename json file
	 /// \params[in] a_pRoot the root node of the behavior tree
	 /// \params[in] a_sFilename the filename of the json file to build the tree from
  void BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename);

private:
	/// \brief recursive Method to parse the json file
	/// \params[in] a_rJsonNode the json node in the file corresponging to a_pNode in the tree
	/// \prarams[in] a_pNode the subroot node to build the tree from
	template <typename T>
	void ParseJson(RefJsonNode a_rJsonNode, T* a_pNode);
};



// include template implementation and specializations
#include "CJsonParser_impl.h"


} // namespace LM

#endif /* _CJSONPARSER_H_ */
