#ifndef _CVALIDATOR_H_
#define _CVALIDATOR_H_

#include <set>
#include "CKernel.h"

namespace LM
{

class CValidator : public CNode
{
 private:
  CKernel* m_pKernel;
  std::set<std::string> m_oIDs;
  std::string m_sSound;

 public:
  CValidator(CKernel* a_pKernel);
  
  void SetSound(const std::string& a_rSound);

  void AddID(const std::string& a_sID);

  void Validate(const std::string& a_sID);

  virtual void ToJson(rapidjson::Value &parent, rapidjson::Document::AllocatorType &allocator);
  
};

} // namespace LM

#endif /* _CVALIDATOR_H_ */
