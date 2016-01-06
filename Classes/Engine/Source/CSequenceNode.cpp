#include "../Include/CSequenceNode.h"

using namespace cocos2d;


namespace LM
{

bool CSequenceNode::OnTouchBegan(Touch* touch, Event* event)
{
  if (m_pCurrentNode != nullptr)
  {
    return m_pCurrentNode->OnTouchBegan(touch, event);
  }

  return false;
}


} // namespace LM
