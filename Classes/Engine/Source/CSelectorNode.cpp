#include "../Include/CSelectorNode.h"

namespace LM
{

bool CSelectorNode::SelectCurrentNode(unsigned int a_iIndex)
{
  if (a_iIndex < m_vChildren.size() && a_iIndex >= 0)
  {
    m_iCurrentNode = a_iIndex;
    return true;
  }

  return false;
}

} // namespace LM
