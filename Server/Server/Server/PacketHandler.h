#pragma once
#include "TestProtocol.h"

#include "Session.h"

bool Handle_CS_LOGIN(shared_ptr<Session> session, CS_LOGIN_PACKET* packet);
bool Handle_CS_CHAT(shared_ptr<Session> session, CS_CHAT_PACKET& packet);


class PacketHandler
{
public:

	static bool HandlerPacket(shared_ptr<Session> session, BYTE* buffer, int len);
	

private:
	
};


