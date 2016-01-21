#include "../Include/CTransitionVisitor.h"
#include "../Include/CSceneNode.h"


namespace LM
{

Result CTransitionVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
  if (pScene)
  {
	  pScene->GetScene()->removeAllChildrenWithCleanup(true);
    return RESULT_PRUNE;
  }
}


} // namespace LM
