#ifndef _CJSONPARSER_H_
#define _CJSONPARSER_H_

#include "rapidjson.h"
#include "document.h"

#include <string>
#include <algorithm>

#include "CCallback.h"
#include "CKernel.h"
#include "CNode.h"
#include "CSequenceNode.h"
#include "CSceneNode.h"
#include "CEntityNode.h"
#include "CSpriteNode.h"
#include "CMenuNode.h"
#include "CLabelNode.h"
#include "CGridNode.h"
#include "CPeerNode.h"
#include "CCameraFeedNode.h"
#include "CValidator.h"
#include "CEditBoxNode.h"
#include "CInfoNode.h"

namespace LM
{

typedef const rapidjson::Value& RefJsonNode;


/// \brief Class responsible for parsing the json file and creating the game tre from it
class CJsonParser
{
private:
	/// \brief the currently processed json document
	rapidjson::Document m_oDocument;

	/// \brief a reference pointer to the game kernel
	CKernel* m_pKernel;

    /// \brief base path for all resources
    std::string m_sBasePath;

 public:
	 CJsonParser(CKernel* a_pKernel);

	 /// \brief Build the tree of root a_pRoot from a_sFilename json file
	 /// \params[in] a_pRoot the root node of the behavior tree
	 /// \params[in] a_sFilename the filename of the json file to build the tree from
    void BuildBehaviorTreeFromFile(CNode* a_pRoot, const std::string& a_sFilename);

    /// \brief Build a new Scene node from a template file 
    void BuildSceneNodeFromFile(CNode* a_pNewScene, const std::string& a_sFileName, int a_iTemplateNumber = 0);

	/// \brief recursive Method to parse the json file
	/// \params[in] a_rJsonNode the json node in the file corresponging to a_pNode in the tree
	/// \prarams[in] a_pNode the subroot node to build the tree from
	template <typename T>
    void ParseJson(RefJsonNode a_rJsonNode, T* a_pNode, bool a_bNodeVisible = true, bool a_bNewScene = false);

	/// returns true if the callback change the visibility of the node to hidden
	bool ParseCallback(RefJsonNode a_rJsonNode, CEntityNode* a_pEntity);
};



// include template implementation and specializations
#include "CJsonParser_impl.h"


} // namespace LM

#endif /* _CJSONPARSER_H_ */
