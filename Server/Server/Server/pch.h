#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

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
#include <mutex>

extern thread_local unsigned int Lthreadid;

#include "TestProtocol.h"
#define PORT_NUM 8888




