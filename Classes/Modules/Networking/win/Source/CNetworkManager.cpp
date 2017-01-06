#include <iostream>
#include <pthread.h>

#include "../Include/CNetworkManager.h"


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"


namespace LM
{

	static const std::string M_BYTES = "LudoMuse:bytes";


	void ProcessBytes(char* recvbuf, int iResult, CNetworkManager* nm)
	{
		printf("Bytes received: %d\n", iResult);
		if (iResult > M_BYTES.size())
		{
			std::string sMessageStart(recvbuf + 1, M_BYTES.size());
			if (sMessageStart == M_BYTES)
			{
				byte oEvent = recvbuf[M_BYTES.size() + 1];
				bytes message(recvbuf + M_BYTES.size() + 2, iResult - M_BYTES.size() - 2);
				nm->m_pKernel->OnReceiving(message, oEvent);
				return;
			}
		}
		int buffIndex = 0;
		while (buffIndex < iResult)
		{
			int messageSize;
			messageSize = recvbuf[buffIndex];
			char* message = new char[messageSize];
			memcpy(message, recvbuf + buffIndex + 1, messageSize);
			buffIndex += messageSize + 1;
			std::cout << "Message size : " << messageSize << std::endl;
			std::cout << "Message read : " << std::string(message, messageSize) << std::endl;
			nm->m_pKernel->OnReceivingMessage(std::string(message, messageSize));
			delete message;
		}
	}


	void* ServerWaitMessages(void* networkManager)
	{

		CNetworkManager* nm = static_cast<CNetworkManager*>(networkManager);

		nm->ClientSocket = INVALID_SOCKET;

		// Accept a client socket
		nm->ClientSocket = accept(nm->ListenSocket, NULL, NULL);
		if (nm->ClientSocket == INVALID_SOCKET) {
			int i = WSAGetLastError();
			printf("accept failed: %d\n", i);
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
				ProcessBytes(recvbuf, iResult, nm);
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



	void* ClientWaitMessages(void* networkManager)
	{
		CNetworkManager* nm = static_cast<CNetworkManager*>(networkManager);

		int recvbuflen = DEFAULT_BUFLEN;
		int iResult;
		char recvbuf[DEFAULT_BUFLEN];
		// Receive data until the server closes the connection
		do {
			iResult = recv(nm->ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				ProcessBytes(recvbuf, iResult, nm);
			}
			else if (iResult == 0)
				printf("Connection closed\n");
			else
				printf("recv failed: %d\n", WSAGetLastError());
		} while (iResult > 0);

		return NULL;
	};



CNetworkManager::CNetworkManager(CKernel* a_pKernel, bool a_bIsServer) : 
	m_pKernel(a_pKernel),
	m_bIsServer(a_bIsServer)
{
	ConnectSocket = NULL;
	ClientSocket = NULL;



	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}

	if (m_bIsServer) {
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
			int i = WSAGetLastError();
			printf("bind failed with error: %d\n", i);
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
		int iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
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

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
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



void CNetworkManager::Send(const char* buff, size_t size)
{
	std::cout << "send message " << buff << std::endl;
	//m_pKernel->OnReceivingMessage(s);
	char* sendBuff = new char[size + 1];
	*sendBuff = size;
	memcpy(sendBuff + 1, buff, size);

	if (m_bIsServer) // Server : use ClientSocket
	{

		// Echo the buffer back to the sender
		int iSendResult = send(ClientSocket, sendBuff, size+1, 0);
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
		iResult = send(ConnectSocket, sendBuff, size+1, 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}

		printf("Bytes Sent: %ld\n", iResult);



	}
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
	if (m_bIsServer)
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
