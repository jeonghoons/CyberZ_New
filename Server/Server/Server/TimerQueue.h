#pragma once
#include "pch.h"
#include <concurrent_priority_queue.h>
#include "Job.h"
#include "ServerService.h"

enum Timer_EventType { EV_NPC_MOVE, EV_UPDATE_ROOM };

struct TimerEvent
{
	bool operator < (const TimerEvent& other) const
	{
		return (other._excuteTime < _excuteTime);
	}

	std::chrono::system_clock::time_point _excuteTime;
	Timer_EventType _type{};
};

class TimerQueue
{
public:
	void TimerRun(shared_ptr<ServerService>& service)
	{
		while (true) {
			TimerEvent event;
			auto currentTime = std::chrono::system_clock::now();
			if (_timerQueue.try_pop(event)) {
				// ť�� ����ų�, �� ����
				if (event._excuteTime > currentTime) {
					// ����ð��� ���� �ȵǾ����Ƿ� ��� ���
					_timerQueue.push(event);
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					continue;
				}
				// �� ����
				switch (event._type)
				{
				case Timer_EventType::EV_NPC_MOVE: {
					shared_ptr<Job> job = make_shared<Job>();
					// job->_jobEvent = new JobEvent();
					JobEvent* jobEvent = new JobEvent();
					jobEvent->Init();
					jobEvent->owner = job;

					PostQueuedCompletionStatus(_iocpInstance->GetHandle(), 1, 0, reinterpret_cast<LPOVERLAPPED>(jobEvent));
				}	break;

				case Timer_EventType::EV_UPDATE_ROOM: {
					shared_ptr<Job> job = make_shared<Job>();
					// job->_jobEvent = new JobEvent();
					JobEvent* jobEvent = new JobEvent();
					jobEvent->Init(); 
					jobEvent->owner = job;

					PostQueuedCompletionStatus(_iocpInstance->GetHandle(), 1, 0, reinterpret_cast<LPOVERLAPPED>(jobEvent));
				}	break;
				default:
					break;
				}
				continue; // ���� �۾� ������
			}
		}

		std::this_thread::sleep_for(1ms);
	}

	void GetInstance(shared_ptr<ServerService>& service)
	{
		_iocpInstance = service->GetIocpInstance();
	}

public:

	shared_ptr<IocpCore> _iocpInstance;
	concurrency::concurrent_priority_queue<TimerEvent> _timerQueue;
};

extern shared_ptr<TimerQueue> GTimerQueue;

