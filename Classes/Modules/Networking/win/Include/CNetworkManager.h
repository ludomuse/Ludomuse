#ifndef _CNETWORKMANAGER_H_
#define _CNETWORKMANAGER_H_

#include "../../../../Engine/Include/CKernel.h"

#include <winsock2.h>
#include <ws2tcpip.h>


namespace LM
{

class CNetworkManager
{

 public:
  CKernel* m_pKernel;
  SOCKET ConnectSocket;
  SOCKET ClientSocket;
  SOCKET ListenSocket;

  struct addrinfo *ptr;
  struct addrinfo *result;

 public:

  CNetworkManager(CKernel* a_pKernel);
  ~CNetworkManager();
  
  void Send(const std::string& s);
  void DiscoverPeers();
  void ConnectTo(const std::string& a_sDeviceName);
  
  
};

} // namespace LM

#endif /* _CNETWORKMANAGER_H_ */
