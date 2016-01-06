#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CNode.h"


namespace LM 
{

class CKernel
{
 private:

  cocos2d::EventListenerTouchOneByOne* m_pEventListener;
  CNode* m_oBehaviorTree;
  

 public:
  CKernel();
  virtual ~CKernel();
  bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};


} // namespace LM

#endif /* _CKERNEL_H_ */
