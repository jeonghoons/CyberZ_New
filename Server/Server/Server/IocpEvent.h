#pragma once

enum class EventType : unsigned char
{
	Accept,
	Recv,
	Send,
	Do_Job
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
class SendBuffer;

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

	vector<shared_ptr<SendBuffer>> sendBuffers;
};

class JobEvent : public IocpEvent
{
public:
	JobEvent() : IocpEvent(EventType::Do_Job) {}


};
