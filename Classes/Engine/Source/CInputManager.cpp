#include "../Include/CInputManager.h"
#include "../Include/CKernel.h"

using namespace cocos2d;

namespace LM
{

CInputManager::CInputManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
  // create a cocos EventListener and bind touchBegan to InputManager
  m_pEventListener = EventListenerTouchOneByOne::create();
  m_pEventListener->onTouchBegan = CC_CALLBACK_2(CInputManager::OnTouchBegan, this);

  auto pEventDispatecher = Director::getInstance()->getEventDispatcher();
  pEventDispatecher->addEventListenerWithFixedPriority(m_pEventListener, 1);
}


bool CInputManager::OnTouchBegan(Touch* a_pTouch, Event* a_pEvent)
{
  //return m_pKernel->GetBehaviorTree()->OnTouchBegan(a_pTouch, a_pEvent); TODO 
	CCLOG("onTouchBegan");
	return m_pKernel->OnTouchBegan(a_pTouch, a_pEvent);
	return true;
}


} // namespace LM
