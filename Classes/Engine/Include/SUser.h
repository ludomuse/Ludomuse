#ifndef _CUSER_H_
#define _CUSER_H_

#include <string>
#include "Modules/Networking/android/Include/LmBytesMessage.h"

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

	virtual void writeOn(bytes* msg) override
	{
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
