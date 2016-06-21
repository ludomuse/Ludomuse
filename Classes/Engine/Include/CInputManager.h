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

  cocos2d::EventListenerTouchOneByOne* GetEventListener();
};

} // namespace LM

#endif /* _CINPUTMANAGER_H_ */
