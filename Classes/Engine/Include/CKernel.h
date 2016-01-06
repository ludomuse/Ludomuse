#ifndef _CKERNEL_H_
#define _CKERNEL_H_


#include "cocos2d.h"
#include "CNode.h"
#include "CInputManager.h"


namespace LM 
{

class CKernel
{
 private:
  CNode* m_pBehaviorTree;
  CInputManager m_oInputManager;

 public:
  CKernel();
  virtual ~CKernel();
  CNode* GetBehaviorTree();
};


} // namespace LM

#endif /* _CKERNEL_H_ */
