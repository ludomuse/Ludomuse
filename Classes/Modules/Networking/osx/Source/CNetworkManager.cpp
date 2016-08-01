#include <iostream>
#include <pthread.h>

#include "../Include/CNetworkManager.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6666"


namespace LM
{

	static const std::string M_BYTES = "LudoMuse:bytes";





CNetworkManager::CNetworkManager(CKernel* a_pKernel, bool a_bIsServer) : 
	m_pKernel(a_pKernel),
	m_bIsServer(a_bIsServer)
{

}



void CNetworkManager::Send(const char* buff, size_t size)
{
}


void CNetworkManager::Send(const std::string& s)
{
	Send(s.c_str(), s.size());
}

void CNetworkManager::Send(const bytes& a_rBytes)
{
	bytes oBytes = a_rBytes;
	const int buffSize = M_BYTES.size() + oBytes.getSize();
	char* buff = new char[buffSize];
	memcpy(buff, M_BYTES.c_str(), M_BYTES.size());
	memcpy(buff + M_BYTES.size(), oBytes.getData(), oBytes.getSize());

	Send(buff, buffSize);

	delete[] buff;
}





void CNetworkManager::DiscoverPeers()
{
	std::cout << "discover peers" << std::endl;
	std::vector<std::string> peers;
	peers.push_back("Jean");
	peers.push_back("Michel");
	peers.push_back("Wendy");
	m_pKernel->OnGettingPeers(peers);
}






void CNetworkManager::ConnectTo(const std::string& a_sDeviceName)
{

}


CNetworkManager::~CNetworkManager()
{
}


} // namespace LM
