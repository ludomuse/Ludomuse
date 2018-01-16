#ifndef _CNETWORKMANAGER_H_
#define _CNETWORKMANAGER_H_

#include "../../../../Engine/Include/CKernel.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef char byte;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;


namespace LM
{

class CNetworkManager
{

 public:
  CKernel* m_pKernel;
  SOCKET ConnectSocket;
  SOCKET ClientSocket;
  SOCKET ListenSocket;

  bool m_bIsServer;

  struct addrinfo *ptr;
  struct addrinfo *result;

  bool m_bConnectionClosed;
  pthread_mutex_t m_oConnectionMutex;


  CNetworkManager(CKernel* a_pKernel, bool a_bIsServer = true);
  ~CNetworkManager();
  
  void Send(const char* buff, size_t size);
  void Send(const std::string& s);
  void Send(const bytes& b);
  void DiscoverPeers();
  void ConnectTo(const std::string& a_sDeviceName);
  
private:
 pthread_t m_oThread;

};

} // namespace LM

#endif /* _CNETWORKMANAGER_H_ */
