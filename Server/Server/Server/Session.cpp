#include "pch.h"
#include "Session.h"
#include "ServerService.h"

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
	switch (iocpEvent->type)
	{
	case EventType::Recv:
		ProcessRecv(numBytes);
		break;
	case EventType::Send:
		break;

	default:
		cout << "Undefined Event" << endl;
		break;
	}
}

void Session::OnConnected()
{
	_connected.store(true);

	GetService()->AddSession(static_pointer_cast<Session>(shared_from_this()));
	
	RegisterRecv();
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.BufferSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_recvEvent, nullptr))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}


}

void Session::ProcessRecv(int numOfBytes)
{
	_recvEvent.owner = nullptr;

	cout << "Recv " << numOfBytes << " Bytes" << endl;

	RegisterRecv();
}


