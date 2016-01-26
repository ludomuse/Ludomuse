#include "../Include/CTransitionVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CSequenceNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CTransitionVisitor::CTransitionVisitor(bool a_bTransitionNext) :
	m_bTransitionNext(a_bTransitionNext)
{
}

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
	  //pScene->GetScene()->removeAllChildrenWithCleanup(true); // TODO
	  bool bSceneExists = false;
	  if (m_bTransitionNext)
	  {
		  // if TransitioNext offset of 1 scene 
		  bSceneExists = pSequence->OffsetCurrentNode(1);
	  }
	  else
	  {
		  // else return to previous scene
		  bSceneExists = pSequence->OffsetCurrentNode(-1);
	  }
	  if (!bSceneExists)
	  {
		  return RESULT_CONTINUE;
	  }
	  CSceneNode* pNewSceneNode = dynamic_cast<CSceneNode*>(pSequence->GetCurrentNode());
	  if (pNewSceneNode)
	  {
		  Scene* pNewScene = pNewSceneNode->CreateScene();
		  pNewSceneNode->init();

		  if (m_bTransitionNext)
		  {
			  Director::getInstance()->replaceScene(TransitionSlideInR::create(1.0f, pNewScene));
		  }
		  else
		  {
			  Director::getInstance()->replaceScene(TransitionSlideInL::create(1.0f, pNewScene));
		  }
	  }
      return RESULT_PRUNE;
  }
}


} // namespace LM
