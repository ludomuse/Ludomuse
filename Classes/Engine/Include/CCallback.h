#ifndef _CCALLBACK_H_
#define _CCALLBACK_H_


#include "CKernel.h"
#include "cocos2d.h"

namespace LM
{

template <class T, typename Arg>
class CCallback
{

typedef  void (T::* fpMemberCallback)(Arg pSender);


private:
  T* m_pCallee;
  fpMemberCallback m_pCallback;
  Arg m_oArgument;


public:
	CCallback(T* a_pCallee, fpMemberCallback a_pCallback, Arg a_oArgument = Arg()) :
		m_pCallee(a_pCallee),
		m_pCallback(a_pCallback),
		m_oArgument(a_oArgument)
	{

	}


	void operator()(Arg a_pSender)
	{
		(m_pCallee->*m_pCallback)(a_pSender);
	}

	void operator()()
	{
		(m_pCallee->*m_pCallback)(m_oArgument);
	}
  
};


} // namespace LM

#endif /* _CCALLBACK_H_ */
