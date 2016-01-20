#ifndef _CINPUTMANAGER_H_
#define _CINPUTMANAGER_H_


#include "cocos2d.h"
#include "CKernel.h"


namespace LM
{


/// \class CInputmanager
/// \ingroup Engine
/// \brief Class responsible for getting user inputs
class CInputManager
{
 private:
  /// \brief a reference to the kernel of the engine
  CKernel* m_pKernel;
  /// \brief listens to cocos2d events
  cocos2d::EventListenerTouchOneByOne* m_pEventListener;

 public:
  CInputManager(CKernel* a_rKernel);
  /// \brief callback called when the user start touching the screen
  /// \details forward the event to the BehaviorTree in the kernel
  /// \returns true on success, false otherwise
  /// \param[in] a_pTouch the touch event
  /// \param[in] a_pEvent the cocos Event corresponding to this touch
  bool OnTouchBegan(cocos2d::Touch* a_pTouch, cocos2d::Event* a_pEvent);
};

} // namespace LM

#endif /* _CINPUTMANAGER_H_ */
