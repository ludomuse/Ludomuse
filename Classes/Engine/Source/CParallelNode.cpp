#include "../Include/CParallelNode.h"

namespace LM
{


CParallelNode::CParallelNode()
{
  
}

bool CParallelNode::OnTouchBegan(Touch* touch, Event* event)
{
  bool bSuccess = true;
  std::vector<CNode*>::iterator itChild;
  for (itChild = m_vChildren.begin(); itChild != m_vChildren.end(); ++it)
  {
    bSuccess = bSuccess && it->OnTouchBegan(Touch* touch, Event* event);
  }
  return bSuccess;
}


} // namespace LM
