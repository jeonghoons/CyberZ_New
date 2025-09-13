#include "pch.h"
#include "Job.h"

HANDLE Job::GetHandle()
{
    return HANDLE();
}

void Job::Dispatch(IocpEvent* iocpEvent, int numBytes)
{
	switch (iocpEvent->type)
	{
	case EventType::Recv:
		
		break;
	case EventType::Send:
		
		break;

	default:
		cout << "Undefined Event" << endl;
		break;
	}
}
