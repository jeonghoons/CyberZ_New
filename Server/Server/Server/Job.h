#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"

class Job : public IocpObject
{
public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numBytes = 0) override;

	void UpdateRoom()
	{

	}

public:
	shared_ptr<Room> _room;
	JobEvent _jobEvent;
};

