#include "../Include/CGotoSceneVisitor.h"
#include "../Include/CSceneNode.h"
#include "../Include/CSequenceNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CGotoSceneVisitor::CGotoSceneVisitor(const std::string& a_rSceneID) :
    m_sSceneID(a_rSceneID)
{
  
}


Result CGotoSceneVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pSceneNode = dynamic_cast<CSceneNode*>(a_pNode);
  if (pSceneNode)
  {
    if (pSceneNode->GetSceneID() == m_sSceneID)
    {
		CSequenceNode* pSequence = dynamic_cast<CSequenceNode*>(pSceneNode->GetParent());
		pSequence->SetCurrentNode(pSceneNode);
      Scene* pNewScene = pSceneNode->CreateScene();
      pSceneNode->init();

      Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5f, pNewScene));
      return RESULT_PRUNE;
    }
  }
  return RESULT_CONTINUE;
}


} // namespace LM
