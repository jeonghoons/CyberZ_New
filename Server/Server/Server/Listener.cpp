#include "pch.h"
#include "Listener.h"
#include "ServerService.h"
#include "IocpEvent.h"
#include "Session.h"
#include "NetAddress.h"

Listener::~Listener()
{
	::closesocket(listenSocket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(listenSocket);
}

void Listener::Dispatch(class IocpEvent* iocpEvent, int numBytes)
{
	if (iocpEvent->type != EventType::Accept)
		exit(-1);

	AcceptEvent* accpetEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(accpetEvent);
}

bool Listener::StartAccept(shared_ptr<ServerService> service)
{
	_service = service;
	if (_service == nullptr)
		return false;

	listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listenSocket == INVALID_SOCKET)
		return false;

	if (_service->GetIocpInstance()->Register(shared_from_this()) == false)
		return false;
	
	if (SOCKET_ERROR == ::bind(listenSocket, reinterpret_cast<SOCKADDR*>(&_service->GetNetAddress().GetSockAddr()), sizeof(SOCKADDR)))
		return false;

	if (SOCKET_ERROR == ::listen(listenSocket, SOMAXCONN))
		return false;

	_acceptOver = new AcceptEvent();
	_acceptOver->owner = shared_from_this();
	RegisterAccept(_acceptOver);

	return true;
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = _service->CreateSession();
	_acceptOver->Init();
	_acceptOver->_session = session;

	DWORD num_bytes{};
	if (false == AcceptEx(listenSocket, session->GetSocket(), session->_recvBuffer.WritePos(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &num_bytes, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			RegisterAccept(_acceptOver);
		}
	}

}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	shared_ptr<Session> session = acceptEvent->_session;

	::setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
		(char*)&listenSocket, sizeof(listenSocket));

	SOCKADDR_IN sockAddress;
	int sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		const int errorCode = ::WSAGetLastError();
		RegisterAccept(acceptEvent);
		return;
	}



	cout << "Client Connected " << endl;

	//

	RegisterAccept(acceptEvent);
}
