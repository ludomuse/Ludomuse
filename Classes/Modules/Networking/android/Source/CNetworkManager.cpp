#include <iostream>

#include "../Include/CNetworkManager.h"


namespace LM
{


CNetworkManager::CNetworkManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
  m_oWifiDirectFacade.addObserver(this);
}

void CNetworkManager::onReceiving(std::string s)
{
  std::cout << s << std::endl;
}

void CNetworkManager::send(std::string s)
{
  m_oWifiDirectFacade.send(s);
}


} // namespace LM

