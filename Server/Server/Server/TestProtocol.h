#pragma once
// #include "pch.h"
#include <DirectXMath.h>
using namespace DirectX;

// Packet Key
enum class SC_PACKET_LIST : unsigned char
{
	SC_LOGIN, SC_LOGOUT, SC_ADD_PLAYER, SC_DELETE_PLAYER
};

enum CS_PACKET_LIST : unsigned char
{
	CS_LOGIN, CS_LOGOUT, CS_CHAT
};
// =======================

#pragma pack (push, 1)
struct PacketHeader
{
	unsigned short size;
	unsigned short type;
};

struct CS_LOGIN_PACKET {
	PacketHeader header;
};

struct CS_LOGOUT_PACKET {
	PacketHeader header;
};

struct CS_CHAT_PACKET {
	PacketHeader header;
	char message[1024];
};

// ------------------------------------------
struct SC_LOGIN_INFO_PACKET {
	
};

#pragma pack (pop)
