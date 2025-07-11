#pragma once
#include "Listener.h"
#include "IocpCore.h"
#include "Session.h"
#include "NetAddress.h"

class ServerService : public enable_shared_from_this<ServerService>
{
public:
	ServerService(NetAddress address, shared_ptr<IocpCore> iocpCore);
	~ServerService();

	bool Start();

	shared_ptr<IocpCore>& GetIocpInstance() { return _iocpCore; }
	NetAddress	GetNetAddress() { return _netAddress; }

	shared_ptr<Session>	CreateSession();
	void			AddSession(shared_ptr<Session> session);
	void			ReleaseSession(shared_ptr<Session> session);

private:
	shared_ptr<Listener>		_listener;
	NetAddress				_netAddress;
	shared_ptr<IocpCore>		_iocpCore;

	unordered_map<int, shared_ptr<Session>> _sessions;

};

