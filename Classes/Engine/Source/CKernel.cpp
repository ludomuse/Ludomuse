#include "../Include/CKernel.h"
#include "../Include/CSequenceNode.h"

using namespace cocos2d;

namespace LM
{

CKernel::CKernel()
{
  m_pEventListener = EventListenerTouchOneByOne::create();
  m_pEventListener->onTouchBegan = CC_CALLBACK_2(CKernel::OnTouchBegan, this);

  m_oBehaviorTree = new CSequenceNode();
}


bool CKernel::OnTouchBegan(Touch* touch, Event* event)
{
  return m_oBehaviorTree->OnTouchBegan(touch, event);
}


CKernel::~CKernel()
{
  delete m_oBehaviorTree;
}

} // namespace LM
