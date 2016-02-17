#include "../Include/CTouchBeganVisitor.h"

#include "../Include/CEntityNode.h"
#include "../Include/CSceneNode.h"

namespace LM
{

CTouchBeganVisitor::CTouchBeganVisitor()
{
  
}


void CTouchBeganVisitor::Traverse(CNode* a_pNode)
{
  if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
  {
    CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
    CSceneNode* pScene = dynamic_cast<CSceneNode*>(a_pNode);
    if (pEntity || pScene)
    {
      for (CNode* itNode : *a_pNode)
      {
        Traverse(itNode);
      }
    }
    else
    {
      Traverse(a_pNode->GetCurrentNode());
    }

    ProcessNodeBottomUp(a_pNode);
  }
}


Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);

  if (pEntity)
  {
    if (pEntity->IsListeningTo("Touch"))
    {
      pEntity->Dispatch("Touch");
      return RESULT_PRUNE;
    }
  }
  
  return RESULT_CONTINUE;
}


} // namespace LM
