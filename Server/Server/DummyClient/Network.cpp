#include "Network.h"

Network::Network() : _recvBuffer(65536)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		exit(-1);
	}
}

Network::~Network()
{
	::closesocket(_socket);
	WSACleanup();
}

bool Network::Connect2Server()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
		return false;

	unsigned long on = 1;
	if (::ioctlsocket(_socket, FIONBIO, &on) == INVALID_SOCKET)
		return false;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(PORT_NUM);

	while (true)
	{
		if (SOCKET_ERROR == ::connect(_socket, (SOCKADDR*)(&serverAddr), sizeof(serverAddr)))
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			break;
		}
	}

	return true;
}
