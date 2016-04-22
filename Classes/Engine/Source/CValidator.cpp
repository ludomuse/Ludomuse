#include "../Include/CValidator.h"
#include "../Include/CCallback.h"
#include "../Include/CSoundManager.h"

namespace LM
{

CValidator::CValidator(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
}


void CValidator::SetSound(const std::string& a_rSound)
{
	m_sSound = a_rSound;
}

void CValidator::AddID(const std::string& a_sID)
{
  m_oIDs.insert(a_sID);
}


void CValidator::Validate(const std::string& a_sID)
{
	int iOldSize = m_oIDs.size();
  std::set<std::string>::iterator itID = m_oIDs.find(a_sID);
  if (itID != m_oIDs.end())
  {
    m_oIDs.erase(itID);
  }


  if (m_oIDs.size() == 0 && iOldSize > m_oIDs.size())
  {
	m_pKernel->ValidateScene(CEvent(), nullptr);
	m_pKernel->SendNetworkMessage("kernel:Validate");

	if (m_sSound != "")
		m_pKernel->m_pSoundManager->PlaySound(m_sSound);
  }
}

}
