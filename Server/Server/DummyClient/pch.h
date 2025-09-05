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
#include <unordered_map>

#include "..//Server/TestProtocol.h"

using namespace std;

#define PORT_NUM 8888

constexpr auto SCREEN_WIDTH = 16;
constexpr auto SCREEN_HEIGHT = 16;

constexpr auto TILE_WIDTH = 45;
constexpr auto NUM_TILE = 64;
// constexpr auto WINDOW_WIDTH = SCREEN_WIDTH * TILE_WIDTH;   // size of window
// constexpr auto WINDOW_HEIGHT = SCREEN_WIDTH * TILE_WIDTH;
constexpr auto WINDOW_WIDTH = 800;   // size of window
constexpr auto WINDOW_HEIGHT = 600;

static int g_left_x;
static int g_top_y;
static int g_myid;
extern sf::RenderWindow* g_window;
extern sf::Font* g_font;
