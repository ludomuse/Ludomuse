#include "../Include/CVisitor.h"


namespace LM
{

CVisitor::CVisitor() : m_pRootNode(nullptr),
                       m_pCurrentNode(nullptr)
{
  
}


CVisitor::~CVisitor()
{
  
}

void CVisitor::Traverse(CNode* a_pNode)
{
  if (ProcessNodeTopDown(a_pNode) == RESULT_CONTINUE)
  {
    for (CNode* itNode : *a_pNode)
    {
      Traverse(itNode);
    }
    ProcessNodeBottomUp(a_pNode);
  }
}


Result CVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  return RESULT_CONTINUE;
}

void CVisitor::ProcessNodeBottomUp(CNode* a_pNode)
{
  
}


} // namespace LM
