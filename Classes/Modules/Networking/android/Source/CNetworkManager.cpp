#include <iostream>

#include "../Include/CNetworkManager.h"


namespace LM
{


CNetworkManager::CNetworkManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
  m_oWifiDirectFacade.addObserver(this);
}


////////////////////////// Callbacks from WifiObserver

void CNetworkManager::onGettingPeers(std::vector<std::string> a_vPeers)
{
  m_pKernel->OnGettingPeers(a_vPeers);
}

void CNetworkManager::onReceiving(std::string s)
{
	m_pKernel->OnReceivingMessage(s);
  m_pKernel->LogMessage("incomming message : " + s);
}



////////////////////////// forward calls to WifiDirectfacade

void CNetworkManager::Send(const std::string& s)
{
  m_oWifiDirectFacade.send(s);
}


void CNetworkManager::DiscoverPeers()
{
  m_oWifiDirectFacade.discoverPeers();
}

void CNetworkManager::ConnectTo(const std::string& a_sDeviceName)
{
  m_oWifiDirectFacade.connectTo(a_sDeviceName);
}


} // namespace LM

