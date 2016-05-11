#ifndef _CCALLBACK_H_
#define _CCALLBACK_H_


#include "CKernel.h"
#include "cocos2d.h"

namespace LM
{


struct SEvent
{

	CNode* m_pSender;
	std::string m_sStringValue;
	bool m_bBoolValue;
	int m_iIntValue;

	SEvent(CNode* a_pSender = nullptr, std::string a_sStringValue = "", bool a_bBoolValue = true, int a_iIntValue = 0) :
		m_pSender(a_pSender),
		m_sStringValue(a_sStringValue),
		m_bBoolValue(a_bBoolValue),
		m_iIntValue(a_iIntValue)
	{

	}

};

template <class T, typename Arg>
class CCallback
{

typedef  void (T::* fpMemberCallback)(Arg pSender, CEntityNode* pTarget);


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


	void operator()(Arg a_pSender, CEntityNode* pTarget = nullptr)
	{
		(m_pCallee->*m_pCallback)(a_pSender, pTarget);
	}

	void operator()(CEntityNode* pTarget = nullptr)
	{
		if (m_pCallback && m_pCallee)
			(m_pCallee->*m_pCallback)(m_oArgument, pTarget);
	}
  
};

typedef CCallback<CKernel, SEvent> CEventCallback;

} // namespace LM

#endif /* _CCALLBACK_H_ */
