#include "pch.h"
#include "thread"
#include "ServerService.h"
#include "IocpCore.h"
#include "Room.h"
#include "TimerQueue.h"

void worker_thread(shared_ptr<ServerService>& service)
{
	while (true)
	{
		service->GetIocpInstance()->Dispatch();
	}
}

void timer_Thread(shared_ptr<ServerService>& service)
{
	GTimerQueue->TimerRun(service);
}

int main()
{
	shared_ptr<ServerService> service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", PORT_NUM),
		make_shared<IocpCore>()
	);

	if (service->Start() == false) {
		cout << "Can't Start" << endl;
		exit(-1);
	}
	else
		cout << "Service Start" << endl;

	
	GRoomManager->CreateRoom();

	GTimerQueue->GetInstance(service);

	vector<thread> threads;
	int num_threads = 8;
	for (int i = 0; i < num_threads; ++i) {
		threads.emplace_back(worker_thread, std::ref(service));
	}
	
	thread timerThread{ timer_Thread, std::ref(service) };
	
	timerThread.join();
	for (thread& t : threads) {
		t.join();
	}


}