#pragma once

enum class EventType : UINT8
{
	Accept,
	Recv,
	Send
};

class IocpEvent : public OVERLAPPED
{

public:
	IocpEvent(EventType _type);

	void Init();

public:
	EventType	type;
	shared_ptr<class IocpObject> owner;
};

class Session;

class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	shared_ptr<Session> _session = nullptr;
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};

class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}
};

