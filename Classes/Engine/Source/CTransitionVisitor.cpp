#include "../Include/CTransitionVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CSequenceNode.h"
#include "../Include/CKernel.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CTransitionVisitor::CTransitionVisitor(CKernel* a_pKernel, bool a_bTransitionNext) :
	m_bTransitionNext(a_bTransitionNext),
	m_pKernel(a_pKernel)
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
	  GotoScene(pSequence);
  }
  return RESULT_PRUNE;
}


void CTransitionVisitor::GotoScene(CSequenceNode* a_pSequence)
{
	bool bSceneExists = false;
	if (m_bTransitionNext)
	{
		// if TransitioNext offset of 1 scene 
		bSceneExists = a_pSequence->OffsetCurrentNode(1);
	}
	else
	{
		// else return to previous scene
		bSceneExists = a_pSequence->OffsetCurrentNode(-1);
	}
	if (!bSceneExists)
	{
		return;
	}
	CSceneNode* pNewSceneNode = dynamic_cast<CSceneNode*>(a_pSequence->GetCurrentNode());
	if (!m_pKernel->PlayerHasScene(pNewSceneNode->GetSceneID()))
	{
		GotoScene(a_pSequence);
		return;
	}
	else if (pNewSceneNode)
	{
		Scene* pNewScene = pNewSceneNode->CreateScene();
		pNewSceneNode->init();

		if (m_bTransitionNext)
		{
			Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, pNewScene));
		}
		else
		{
			Director::getInstance()->replaceScene(TransitionSlideInL::create(0.5f, pNewScene));
		}
	}

}

} // namespace LM
