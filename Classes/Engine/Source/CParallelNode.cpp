#include "../Include/CParallelNode.h"


using namespace cocos2d;

namespace LM
{


CParallelNode::CParallelNode()
{
  
}

bool CParallelNode::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
  bool bSuccess = true;
  std::vector<CNode*>::iterator itChild;
  // forward the OnTouchBegan event to each child
  for (itChild = m_vChildren.begin(); itChild != m_vChildren.end(); ++itChild)
  {
    bSuccess = bSuccess && (*itChild)->OnTouchBegan(a_pTouch, a_pEvent);
  }
  // if each child's OnTouchbegan succeeds then success
  return bSuccess;
}


} // namespace LM
