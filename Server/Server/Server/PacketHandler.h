#pragma once
#include "TestProtocol.h"

#include "Session.h"
#include "SendBuffer.h"

bool Handle_CS_LOGIN(shared_ptr<Session> session, CS_LOGIN_PACKET* packet);
bool Handle_CS_CHAT(shared_ptr<Session> session, CS_CHAT_PACKET* packet);


class PacketHandler
{
public:

	static bool HandlePacket(shared_ptr<Session> session, BYTE* buffer, int len);
	static shared_ptr<SendBuffer> MakePacket(SC_PACKET_LIST type);
};


