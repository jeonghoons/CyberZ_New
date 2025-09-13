#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"

class Job : public IocpObject
{
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int numBytes = 0) override;


};

