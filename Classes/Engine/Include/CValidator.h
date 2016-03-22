#ifndef _CVALIDATOR_H_
#define _CVALIDATOR_H_

#include <set>
#include "CKernel.h"
#include "CEntityNode.h"

namespace LM
{

class CValidator : public CEntityNode
{
 private:
  CKernel* m_pKernel;
  std::set<std::string> m_oIDs;

 public:
  CValidator(CKernel* a_pKernel);
  
  void AddID(const std::string& a_sID);

  void Validate(const std::string& a_sID);
  
};

} // namespace LM

#endif /* _CVALIDATOR_H_ */
