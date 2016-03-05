#include <iostream>

#include "../Include/CNetworkManager.h"

namespace LM
{

CNetworkManager::CNetworkManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
  
}

void CNetworkManager::Send(const std::string& s)
{
  std::cout << "send message " << s << std::endl;
  m_pKernel->OnReceivingMessage(s);
}

void CNetworkManager::DiscoverPeers()
{
  std::cout << "discover peers" << std::endl;
  std::vector<std::string> peers;
  peers.push_back("PEER1");
  peers.push_back("Holla");
  peers.push_back("rtes");
  m_pKernel->OnGettingPeers(peers);
}

void CNetworkManager::ConnectTo(const std::string& a_sDeviceName)
{
  std::cout << "connect to " << a_sDeviceName << std::endl;
}


} // namespace LM
