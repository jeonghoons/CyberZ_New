#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType _type) : type(_type)
{
	Init();
}

void IocpEvent::Init()
{
	OVERLAPPED::Internal = 0;
	OVERLAPPED::InternalHigh = 0;
	OVERLAPPED::Offset = 0;
	OVERLAPPED::OffsetHigh = 0;
	OVERLAPPED::hEvent = nullptr;
}
