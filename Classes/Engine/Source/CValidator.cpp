#include "../Include/CValidator.h"
#include "../Include/CCallback.h"

namespace LM
{

CValidator::CValidator(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
}

void CValidator::AddID(const std::string& a_sID)
{
  m_oIDs.insert(a_sID);
}


void CValidator::Validate(const std::string& a_sID)
{
  std::set<std::string>::iterator itID = m_oIDs.find(a_sID);
  if (itID != m_oIDs.end())
  {
    m_oIDs.erase(itID);
  }

  if (m_oIDs.size() == 0)
  {
    m_pKernel->ValidateScene(CEvent(nullptr, "", true), nullptr);
  }
}

}
