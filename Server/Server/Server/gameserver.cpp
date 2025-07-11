#include "pch.h"
#include "thread"
#include "ServerService.h"

void worker_thread(shared_ptr<ServerService>& service)
{
	while (true)
	{
		service->GetIocpInstance()->Dispatch();
	}
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


	vector<thread> _threads;
	int num_threads = 4;
	for (int i = 0; i < num_threads; ++i) {
		_threads.emplace_back(worker_thread, std::ref(service));
	}

	for (thread& t : _threads) {
		t.join();
	}


}