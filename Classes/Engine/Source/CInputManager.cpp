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

  m_pKeyListener = EventListenerKeyboard::create();
  m_pKeyListener->onKeyReleased = CC_CALLBACK_2(CInputManager::OnKeyReleased, this);

  auto pEventDispatcher = Director::getInstance()->getEventDispatcher();
  pEventDispatcher->addEventListenerWithFixedPriority(m_pEventListener, 1);
  pEventDispatcher->addEventListenerWithFixedPriority(m_pKeyListener, 1);

}


EventListenerTouchOneByOne* CInputManager::GetEventListener()
{
	return m_pEventListener;
}


void CInputManager::OnKeyReleased(cocos2d::EventKeyboard::KeyCode a_oKeyCode, cocos2d::Event* a_pEvent)
{
	CCLOG("key released : %d", a_oKeyCode);
	if (a_oKeyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		if (m_pKernel->m_bDebugMode)
		{
			// toggle the quick jump-to-scene box
			m_pKernel->m_pCurrentScene->ToggleQuickBox();
		}
	}
}


} // namespace LM
