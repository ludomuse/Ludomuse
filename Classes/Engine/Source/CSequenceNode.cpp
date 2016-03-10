#include "../Include/CSequenceNode.h"

using namespace cocos2d;


namespace LM
{




void CSequenceNode::Finish()
{
  // increment the index of the current node in the sequence
  // if the current node was the last in the children, finish the sequence
  m_iCurrentNode++;
  if (m_iCurrentNode >= m_vChildren.size())
  {
    m_pParent->Finish();
  }
}


} // namespace LM
