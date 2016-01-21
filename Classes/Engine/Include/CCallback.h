#ifndef _CCALLBACK_H_
#define _CCALLBACK_H_


#include "CKernel.h"
#include "cocos2d.h"

namespace LM
{


typedef  void (CKernel::* FPKernelCallback)(cocos2d::Ref* pSender);


class CCallback
{

private:
  CKernel* m_pKernel;
  FPKernelCallback m_pCallback;


public:
  CCallback(CKernel* a_pKernel, FPKernelCallback a_pCallback);
  void operator()(cocos2d::Ref* a_pSender);
  
};


} // namespace LM

#endif /* _CCALLBACK_H_ */
