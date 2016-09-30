#include <iostream>

#include "../Include/CNetworkManager.h"


namespace LM
{


CNetworkManager::CNetworkManager(CKernel* a_pKernel, bool a_bIsServer) : m_pKernel(a_pKernel)
{
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


void CNetworkManager::onReceivingMsg(bytes byteArray)
{
}

////////////////////////// forward calls to WifiDirectfacade

void CNetworkManager::Send(const std::string& s)
{
}

void CNetworkManager::Send(bytes b)
{
}


void CNetworkManager::DiscoverPeers()
{
}

void CNetworkManager::ConnectTo(const std::string& a_sDeviceName)
{
}


} // namespace LM

