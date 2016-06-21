#include "../Include/CInputManager.h"
#include "../Include/CKernel.h"

using namespace cocos2d;

namespace LM
{

CInputManager::CInputManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
  // create a cocos EventListener and bind touchBegan to InputManager
  m_pEventListener = EventListenerTouchOneByOne::create();

  m_pEventListener->onTouchBegan = CC_CALLBACK_2(CKernel::OnTouchBegan, m_pKernel);
  m_pEventListener->onTouchEnded = CC_CALLBACK_2(CKernel::OnTouchEnd, m_pKernel);
  m_pEventListener->onTouchMoved = CC_CALLBACK_2(CKernel::OnTouchMove, m_pKernel);

  auto pEventDispatcher = Director::getInstance()->getEventDispatcher();
  pEventDispatcher->addEventListenerWithFixedPriority(m_pEventListener, 1);


}


EventListenerTouchOneByOne* CInputManager::GetEventListener()
{
	return m_pEventListener;
}

} // namespace LM
