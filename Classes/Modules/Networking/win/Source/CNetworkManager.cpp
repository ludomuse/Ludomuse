#include <iostream>
#include <pthread.h>

#include "../Include/CNetworkManager.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"


namespace LM
{

CNetworkManager::CNetworkManager(CKernel* a_pKernel) : m_pKernel(a_pKernel)
{
	ConnectSocket = NULL;
	ClientSocket = NULL;
}

void CNetworkManager::Send(const std::string& s)
{
  std::cout << "send message " << s << std::endl;
  //m_pKernel->OnReceivingMessage(s);

  if (m_pKernel->GetCurrentPlayer() == 0) // Server : use ClientSocket
  {

	  // Echo the buffer back to the sender
	  int iSendResult = send(ClientSocket, s.c_str(), (int)strlen(s.c_str()), 0);
	  if (iSendResult == SOCKET_ERROR) {
		  printf("send failed: %d\n", WSAGetLastError());
		  closesocket(ClientSocket);
		  WSACleanup();
		  return;
	  }
	  printf("Bytes sent: %d\n", iSendResult);

  }
  else // client use ConnectSocket
  {

	  int iResult;

	  // Send an initial buffer
	  iResult = send(ConnectSocket, s.c_str(), (int)strlen(s.c_str()), 0);
	  if (iResult == SOCKET_ERROR) {
		  printf("send failed: %d\n", WSAGetLastError());
		  closesocket(ConnectSocket);
		  WSACleanup();
		  return;
	  }

	  printf("Bytes Sent: %ld\n", iResult);



  }
}



void* ServerWaitMessages(void* networkManager)
{

	CNetworkManager* nm = static_cast<CNetworkManager*>(networkManager);

	nm->ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	nm->ClientSocket = accept(nm->ListenSocket, NULL, NULL);
	if (nm->ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(nm->ListenSocket);
		WSACleanup();
		return NULL;
	}


	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(nm->ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			nm->m_pKernel->OnReceivingMessage(std::string(recvbuf));
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(nm->ClientSocket);
			WSACleanup();
			return NULL;
		}

	} while (iResult > 0);

	return NULL;
};



void CNetworkManager::DiscoverPeers()
{
  std::cout << "discover peers" << std::endl;
  std::vector<std::string> peers;
  peers.push_back("PEER1");
  peers.push_back("Holla");
  peers.push_back("rtes");
  m_pKernel->OnGettingPeers(peers);

  WSADATA wsaData;

  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
	  printf("WSAStartup failed: %d\n", iResult);
	  return;
  }

  if (m_pKernel->GetCurrentPlayer() == 0) {
	// create a server socket
	  struct addrinfo hints;
	  result = ptr = NULL;

	  ZeroMemory(&hints, sizeof(hints));
	  hints.ai_family = AF_INET;
	  hints.ai_socktype = SOCK_STREAM;
	  hints.ai_protocol = IPPROTO_TCP;
	  hints.ai_flags = AI_PASSIVE;

	  // Resolve the local address and port to be used by the server
	  int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	  if (iResult != 0) {
		  printf("getaddrinfo failed: %d\n", iResult);
		  WSACleanup();
		  return;
	  }

	  ListenSocket = INVALID_SOCKET;


	  // Create a SOCKET for the server to listen for client connections
	  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);


	  if (ListenSocket == INVALID_SOCKET) {
		  printf("Error at socket(): %ld\n", WSAGetLastError());
		  freeaddrinfo(result);
		  WSACleanup();
		  return;
	  }

	  iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	  if (iResult == SOCKET_ERROR) {
		  printf("bind failed with error: %d\n", WSAGetLastError());
		  freeaddrinfo(result);
		  closesocket(ListenSocket);
		  WSACleanup();
		  return;
	  }

	  freeaddrinfo(result);

	  if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		  printf("Listen failed with error: %ld\n", WSAGetLastError());
		  closesocket(ListenSocket);
		  WSACleanup();
		  return;
	  }

	  pthread_t thread;
	  pthread_create(&thread, NULL, &ServerWaitMessages, this);
  }



  else 
  {
	  // create a client socket
	  struct addrinfo hints;
	  result = ptr = NULL;

	  ZeroMemory(&hints, sizeof(hints));
	  hints.ai_family = AF_UNSPEC;
	  hints.ai_socktype = SOCK_STREAM;
	  hints.ai_protocol = IPPROTO_TCP;
	  int iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
	  if (iResult != 0)
	  {
		  std::cout << "getaddrinfo failed with code " << iResult << std::endl;
		  WSACleanup();
		  return;
	  }

	  ConnectSocket = INVALID_SOCKET;

	  ptr = result;

	  ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	  if (ConnectSocket == INVALID_SOCKET)
	  {
		  std::cout << "error at socket() : " << WSAGetLastError() << std::endl;
		  WSACleanup();
		  return;
	  }

  }
}




void* ClientWaitMessages(void* networkManager)
{
	CNetworkManager* nm = static_cast<CNetworkManager*>(networkManager);

	int recvbuflen = DEFAULT_BUFLEN;
	int iResult;
	char recvbuf[DEFAULT_BUFLEN];
	// Receive data until the server closes the connection
	do {
		int iResult = recv(nm->ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			nm->m_pKernel->OnReceivingMessage(std::string(recvbuf));
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	return NULL;
};



void CNetworkManager::ConnectTo(const std::string& a_sDeviceName)
{
  // connect to socket
	if (m_pKernel->GetCurrentPlayer() != 0)
	{
		// Connect to server.
		int iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			int i = WSAGetLastError();
			std::cout << "error at socket() : " << i << std::endl;
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return;
		}

		pthread_t thread;
		pthread_create(&thread, NULL, &ClientWaitMessages, this);
	}
}


CNetworkManager::~CNetworkManager()
{
	if (m_pKernel->GetCurrentPlayer() == 0)
	{
		// shutdown the send half of the connection since no more data will be sent
		int iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}

		// cleanup
		closesocket(ClientSocket);
		WSACleanup();

		return;
	}
	else
	{
		// shutdown the connection for sending since no more data will be sent
		// the client can still use the ConnectSocket for receiving data
		int iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}

		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();

		return;
	}
}


} // namespace LM
