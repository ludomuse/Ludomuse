#ifndef _CUSER_H_
#define _CUSER_H_

#include <string>


namespace LM
{

  enum EUserSex
  {
    MALE,
    FEMALE
  };
  
  struct SUser
  {
    int m_iPlayerID;
    std::string m_sName;
    EUserSex m_eSex;
  };
  
  
} // namesepace LM

#endif /* _CUSER_H_ */
