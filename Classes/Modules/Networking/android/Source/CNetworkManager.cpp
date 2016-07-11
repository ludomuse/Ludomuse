#include <iostream>

#include "../Include/CNetworkManager.h"


namespace LM
{


CNetworkManager::CNetworkManager(CKernel* a_pKernel, bool a_bIsServer) : m_pKernel(a_pKernel)
{
  m_oWifiDirectFacade.addObserver(this);
  CCLOG("<h1> A </h1>");
  LmJniJavaFacade::getApplicationDirectory();
  CCLOG("<h1> B </h1>");
  m_oWifiDirectFacade.setServerTempFileName(APP_DIR + "/tempPic.jpg");
  CCLOG("<h1> C </h1>");
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
	m_pKernel->OnReceiving(byteArray, _event);
}

////////////////////////// forward calls to WifiDirectfacade

void CNetworkManager::Send(const std::string& s)
{
  m_oWifiDirectFacade.send(s);
}

void CNetworkManager::Send(bytes b)
{
	m_oWifiDirectFacade.sendBytes(b);
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

