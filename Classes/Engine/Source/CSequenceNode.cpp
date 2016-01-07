#include "../Include/CSequenceNode.h"

using namespace cocos2d;


namespace LM
{

bool CSequenceNode::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
  if (m_iCurrentNode != -1)
  {
    return m_vChildren[m_iCurrentNode]->OnTouchBegan(a_pTouch, a_pEvent);
  }

  return false;
}


} // namespace LM
