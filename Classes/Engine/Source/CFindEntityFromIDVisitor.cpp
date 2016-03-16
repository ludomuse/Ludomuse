#include "../Include/CFindEntityFromIDVisitor.h"

namespace LM
{

CFindEntityFromIDVisitor::CFindEntityFromIDVisitor(const Desc<CNode>& a_pEntity, const std::string& a_sID) :
    CFindEntityVisitor(a_pEntity, ""),
    m_sID(a_sID)
{
  
}


Result CFindEntityFromIDVisitor::ProcessNodeTopDown(CNode* a_pNode)
{
  CEntityNode* pEntity = dynamic_cast<CEntityNode*>(a_pNode);
  if (pEntity)
  {
    if (pEntity->GetID() == m_sID)
    {
      m_pEntityToFind.Set(pEntity);
      return RESULT_PRUNE;
    }
  }

  return RESULT_CONTINUE;
}


} // namespace LM
