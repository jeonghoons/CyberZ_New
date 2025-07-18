#pragma once
// #include "pch.h"
#include <DirectXMath.h>
using namespace DirectX;

struct PacketHeader
{
	unsigned short size;
	unsigned short type;
};

// Packet Key

enum CS_PACKET_LIST : unsigned short
{
	CS_LOGIN, CS_LOGOUT, CS_CHAT
};

// =======================

enum class SC_PACKET_LIST : unsigned short
{
	SC_LOGIN, SC_LOGOUT, SC_ADD_PLAYER, SC_DELETE_PLAYER
};


#pragma pack (push, 1)
struct CS_LOGIN_PACKET {
	PacketHeader header;
};

struct CS_LOGOUT_PACKET {
	PacketHeader header;
};

struct CS_CHAT_PACKET {
	PacketHeader header;
	string Message;
};

// ------------------------------------------
struct SC_LOGIN_INFO_PACKET {
	
};

#pragma pack (pop)
