#include "pch.h"
#include "PacketHandler.h"

std::function<bool(shared_ptr<Session>&, BYTE*, int)> GPacketHandler[UINT16_MAX];

bool Handle_CS_LOGIN(shared_ptr<Session>& session, CS_LOGIN_PACKET& packet)
{
	CS_LOGIN_PACKET loginPkt;
	return true;
}

bool Handle_CS_CHAT(shared_ptr<Session>& session, CS_CHAT_PACKET& packet)
{
	CS_CHAT_PACKET chatPkt;
	
	cout << "Client [" << session->GetId() << "] : " << packet.Message << endl;
	return true;
}