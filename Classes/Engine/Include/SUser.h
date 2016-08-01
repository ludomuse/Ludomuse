#ifndef _CUSER_H_
#define _CUSER_H_

#include <string>
#include "Classes/Modules/Networking/android/Include/LmBytesMessage.h"

namespace LM
{

  enum EUserSex
  {
    MALE = 0,
    FEMALE = 1
  };
  
  struct SUser : LmSerializable
  {
    int m_iPlayerID;
    std::string m_sName;
    EUserSex m_eSex;

	volatile bool m_bWaiting;

	bool m_bGameEnded;

	SUser() : m_iPlayerID(0), m_sName(""), m_eSex(EUserSex::MALE), m_bWaiting(false), m_bGameEnded(false)
	{}

	virtual void writeOn(bytes* msg) override
	{
		if (m_sName == "")
		{
			m_sName = "noname";
		}
		*msg << m_iPlayerID << m_sName << m_eSex;
	}

	virtual void readOn(bytes* msg) override
	{
		*msg >> m_iPlayerID;
		CCLOG("SUser::readOn %d", m_iPlayerID);
		*msg >> m_sName;
		int i;
		*msg >> i;
		m_eSex = EUserSex(i);
	}
  };
  
  
} // namesepace LM

#endif /* _CUSER_H_ */
