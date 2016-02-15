#ifndef _CCALLBACK_H_
#define _CCALLBACK_H_


#include "CKernel.h"
#include "cocos2d.h"

namespace LM
{

template <class T>
class CCallback
{

typedef  void (T::* fpMemberCallback)(cocos2d::Ref* pSender);


private:
  T* m_pCallee;
  fpMemberCallback m_pCallback;


public:
	CCallback(T* a_pCallee, fpMemberCallback a_pCallback) :
		m_pCallee(a_pCallee),
		m_pCallback(a_pCallback)
	{

	}


	void operator()(cocos2d::Ref* a_pSender)
	{
		(m_pCallee->*m_pCallback)(a_pSender);
	}
  
};


} // namespace LM

#endif /* _CCALLBACK_H_ */
