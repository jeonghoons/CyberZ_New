#pragma once
#include "IocpCore.h"

class ServerService;
class AcceptEvent;

class Listener : public IocpObject
{
	
public:
	Listener() = default;
	~Listener();

	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numBytes = 0) override;


public:
	bool StartAccept(shared_ptr<ServerService> service);


private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET _listenSocket;
	SOCKET clientSocket;
	
	shared_ptr<ServerService> _service;
	AcceptEvent*  _acceptOver;
};

