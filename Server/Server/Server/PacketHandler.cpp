#include "pch.h"
#include "PacketHandler.h"
#include "SendBuffer.h"
#include "Room.h"
#include "Player.h"

bool Handle_CS_LOGIN(shared_ptr<Session> session, CS_LOGIN_PACKET* packet)
{
	// CS_LOGIN_PACKET* loginPkt = reinterpret_cast<CS_LOGIN_PACKET*>(buffer);

	shared_ptr<Player> player = make_shared<Player>(session);

	GRoom->EnterRoom(player);
	

	return true;
}



bool Handle_CS_CHAT(shared_ptr<Session> session, CS_CHAT_PACKET* packet)
{
	char message[1024] = { '\0', };
	memcpy(message, packet->message, packet->header.size - sizeof(PacketHeader));
	cout << "Client [" << session->GetId() << "] : " << message << endl;
	


	SC_CHAT_PACKET cPacket;
	strcpy_s(cPacket.message, message);
	cPacket.header.size = packet->header.size;
	cPacket.header.type = SC_PACKET_LIST::SC_CHAT;

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	sendBuffer->CopyData(&cPacket, cPacket.header.size);

	
	GRoom->Broadcast(sendBuffer);

	return true;
}

bool PacketHandler::HandlePacket(shared_ptr<Session> session, BYTE* buffer, int len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	
	switch (header->type)
	{
	case CS_PACKET_LIST::CS_LOGIN:
		Handle_CS_LOGIN(session, reinterpret_cast<CS_LOGIN_PACKET*>(buffer));
		break;
	case CS_PACKET_LIST::CS_CHAT:
		Handle_CS_CHAT(session, reinterpret_cast<CS_CHAT_PACKET*>(buffer));
		break;
	default:
		return false;
	}


	return true;
}

shared_ptr<SendBuffer> PacketHandler::MakePacket(SC_PACKET_LIST type)
{
	return shared_ptr<SendBuffer>();

	switch (type)
	{
	case SC_LOGIN:
		break;
	case SC_LOGOUT:
		break;
	case SC_ADD_PLAYER:
		break;
	case SC_DELETE_PLAYER:
		break;
	case SC_CHAT:
		break;
	default:
		break;
	}
}
