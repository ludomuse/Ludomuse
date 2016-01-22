#include "../Include/CTransitionVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CSequenceNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

Result CTransitionVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  // pSequence is the father node of all the scenes. 
  // transition will be done on the between the current scene in the node 
  // (CSequenceNode::m_iCurrentNode) and the next node
  CSequenceNode* pSequence = dynamic_cast<CSequenceNode*>(a_pNode);
  CSceneNode* pScene = nullptr;
  if (pSequence)
  {
	  pScene = dynamic_cast<CSceneNode*>(pSequence->GetCurrentNode());
  }
  if (pScene)
  {
	  pScene->GetScene()->removeAllChildrenWithCleanup(true);

	  if (!pSequence->NextNode())
	  {
		  return RESULT_CONTINUE;
	  }
	  CSceneNode* pNewSceneNode = dynamic_cast<CSceneNode*>(pSequence->GetCurrentNode());
	  if (pNewSceneNode)
	  {
		  Scene* pNewScene = pNewSceneNode->CreateScene();
		  pNewSceneNode->init();

		  Director::getInstance()->replaceScene(
			  TransitionSlideInR::create(0.5f, pNewScene));
	  }
      return RESULT_PRUNE;
  }
}


} // namespace LM
