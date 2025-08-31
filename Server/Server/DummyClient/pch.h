#pragma once
#define SFML_STATIC 1

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "..//Server/TestProtocol.h"

using namespace std;

#define PORT_NUM 8888

