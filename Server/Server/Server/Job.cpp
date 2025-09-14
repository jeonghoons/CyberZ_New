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
	case EventType::Do_Job:
		cout << "Do_Job" << endl;
		break;
	
	case EventType::Do_Timer:
		cout << "Do_Timer" << endl;
		break;

	default:
		cout << "Undefined Event" << endl;
		break;
	}
}
