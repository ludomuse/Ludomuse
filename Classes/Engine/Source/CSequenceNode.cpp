#include "../Include/CSequenceNode.h"

using namespace cocos2d;


namespace LM
{

bool CSequenceNode::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
  if (m_pCurrentNode != nullptr)
  {
    return m_pCurrentNode->OnTouchBegan(a_pTouch, a_pEvent);
  }

  return false;
}


} // namespace LM
