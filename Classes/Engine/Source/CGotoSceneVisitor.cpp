#include "../Include/CGotoSceneVisitor.h"
#include "../Include/CSceneNode.h"

#include "cocos2d.h"

using namespace cocos2d;

namespace LM
{

CGotoSceneVisitor::CGotoSceneVisitor(const std::string& a_sSceneID) :
    m_sSceneID(a_sSceneID)
{
  
}


Result CGotoSceneVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pSceneNode = dynamic_cast<CSceneNode*>(a_pNode);
  if (pSceneNode)
  {
    if (pSceneNode->GetSceneID() == m_sSceneID)
    {
      Scene* pNewScene = pSceneNode->CreateScene();
      pSceneNode->init();

      Director::getInstance()->replaceScene(TransitionPageTurn::create(1.5f, pNewScene, false));
      return RESULT_PRUNE;
    }
  }
  return RESULT_CONTINUE;
}


} // namespace LM
