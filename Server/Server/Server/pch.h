#pragma once

#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <iostream> 
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "MSWSock.lib")

using namespace std;

#include <memory>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <unordered_map>
#include <thread>

extern thread_local unsigned int Lthreadid;

#define PORT_NUM 8888




