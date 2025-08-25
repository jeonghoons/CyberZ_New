#pragma once
#include "RecvBuffer.h"
#include "IocpCore.h"
#include "NetAddress.h"
#include "IocpEvent.h"
#include "SendBuffer.h"

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
	shared_ptr<ServerService> GetService() { return _service.lock(); }
	SOCKET	GetSocket() { return _socket; }
	void		SetNetAddress(NetAddress address) { _netAddress = address; }
	bool		IsConnected() { return _connected; }
	void		SetId(int num) { _cid = num; }
	int		GetId() { return _cid; }

public:
	virtual void	OnConnected();
	void			Disconnect(const WCHAR* cause);

	void			Send(shared_ptr<SendBuffer> sendBuffer);

	int			ProcessData(BYTE* buffer, int len);
	void			ProcessPacket(BYTE* buffer, int len);

private:
	void			RegisterRecv();
	void			ProcessRecv(int numOfBytes);

	void			RegisterSend();
	void			ProcessSend(int numOfBytes);

public:
	mutex		_lock;
	RecvBuffer	_recvBuffer;
	

	queue<shared_ptr<SendBuffer>> _sendQueue;
	atomic<bool>		_sendRegistered = false;

private:
	weak_ptr<ServerService> _service;
	SOCKET _socket;
	NetAddress		_netAddress = {};
	atomic<bool>		_connected = false;
	unsigned int		_cid = {};

	RecvEvent		_recvEvent;
	SendEvent		_sendEvent;

};

