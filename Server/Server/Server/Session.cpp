#include "pch.h"
#include "Session.h"

Session::Session() : _recvBuffer(1024)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

Session::~Session()
{
	if (_socket != INVALID_SOCKET)
		::closesocket(_socket);

	cout << "~Session" << endl;

	_socket = INVALID_SOCKET;
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int numBytes)
{
}
