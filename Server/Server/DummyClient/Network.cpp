#include "pch.h"
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
	// ::closesocket(_socket);
	WSACleanup();
}

bool Network::Connect2Server()
{

	wcout.imbue(locale("korean"));
	sf::Socket::Status status = _socket.connect("127.0.0.1", PORT_NUM);
	_socket.setBlocking(false);

	if (status != sf::Socket::Done) {
		wcout << L"서버와 연결할 수 없습니다.\n";
		exit(-1);
	}

	return true;
}
