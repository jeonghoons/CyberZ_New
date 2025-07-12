#include "pch.h"
#include "ServerService.h"

ServerService::ServerService(NetAddress address, shared_ptr<IocpCore> iocpCore)
	: _netAddress(address), _iocpCore(iocpCore)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		exit(-1);
	}
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	_listener = make_shared<Listener>();
	if (_listener == nullptr)
		return false;

	shared_ptr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;

	return true;
}

shared_ptr<Session> ServerService::CreateSession()
{
	shared_ptr<Session> session = make_shared<Session>();
	session->SetService(shared_from_this());

	if (_iocpCore->Register(session) == false)
		return nullptr;



	return session;
}

void ServerService::AddSession(shared_ptr<Session> session)
{
	
}

void ServerService::ReleaseSession(shared_ptr<Session> session)
{

}
