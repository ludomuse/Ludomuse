#include "../Include/CTouchBeganVisitor.h"

namespace LM
{

CTouchBeganVisitor::CTouchBeganVisitor()
{
  
}

Result CTouchBeganVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  return RESULT_PRUNE;
}


} // namespace LM
