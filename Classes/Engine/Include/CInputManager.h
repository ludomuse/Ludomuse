#ifndef _CINPUTMANAGER_H_
#define _CINPUTMANAGER_H_


#include "cocos2d.h"


namespace LM
{

class CKernel;


class CInputManager
{
 private:
  CKernel& m_rKernel;
  cocos2d::EventListenerTouchOneByOne* m_pEventListener;

 public:
  CInputManager(CKernel& a_rKernel);
  bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
};

} // namespace LM

#endif /* _CINPUTMANAGER_H_ */
