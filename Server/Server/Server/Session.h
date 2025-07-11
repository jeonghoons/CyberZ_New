#pragma once
#include "RecvBuffer.h"
#include "IocpCore.h"

class ServerService;

class Session : public IocpObject
{
	
public:
	Session();
	virtual ~Session();

	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numBytes = 0) override;


public:
	void		SetService(shared_ptr<ServerService> service) { _service = service; }
	SOCKET	GetSocket() { return _socket; }

	 
	RecvBuffer	_recvBuffer;
private:
	weak_ptr<ServerService> _service;
	SOCKET _socket;
		
};

