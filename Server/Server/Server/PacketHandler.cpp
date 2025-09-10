#include "pch.h"
#include "PacketHandler.h"
#include "SendBuffer.h"
#include "Room.h"
#include "Player.h"

bool Handle_CS_LOGIN(shared_ptr<Session> session, CS_LOGIN_PACKET* packet)
{
	// DB �Ǵ� ����
	int playerId = session->GetId();
	shared_ptr<Player> player = make_shared<Player>(session);
	session->_currPlayer = player;

	// GRoom->EnterRoom(player);

	GRoomManager->EnterPlayer(player);

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

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(cPacket.header.size);
	sendBuffer->CopyData(&cPacket, cPacket.header.size);

	
	// GRoom->Broadcast(sendBuffer);

	return true;
}

bool Handle_CS_MOVE(shared_ptr<Session> session, CS_MOVE_PACKET* packet)
{
	
	// GRoom->PlayerMove(GRoom->Id2Player(session->GetId()), packet->direction, packet->move_time);
	
	auto room = session->_currPlayer->GetCurrentRoom();
	room->PlayerMove(session->_currPlayer, packet->direction, packet->move_time);

	return true;
}



bool PacketHandler::ProcessPacket(shared_ptr<Session> session, BYTE* buffer, int len)
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
	case CS_PACKET_LIST::CS_MOVE:
		Handle_CS_MOVE(session, reinterpret_cast<CS_MOVE_PACKET*>(buffer));
		break;
	default:
		return false;
	}


	return true;
}
//===================================================================================
//===================================================================================
shared_ptr<SendBuffer> PacketHandler::MakePacket(shared_ptr<Session> session, SC_PACKET_LIST type)
{
	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(4096);
	
	switch (type)
	{
	case SC_ADD_PLAYER:
		MAKE_SC_ADD_PLAYER(session, sendBuffer);
		break;
	case SC_LOGOUT:
		break;
	case SC_REMOVE_PLAYER:
		MAKE_SC_REMOVE_PLAYER(session, sendBuffer);
		break;
	case SC_CHAT:
		break;
	default:
		break;
	}

	return sendBuffer;
}

bool MAKE_SC_ADD_PLAYER(shared_ptr<Session> session, shared_ptr<SendBuffer> buffer)
{
	shared_ptr<Player> player = session->_currPlayer;

	SC_ADD_PLAYER_PACKET packet;
	packet.header = { sizeof(packet), SC_ADD_PLAYER };
	packet.player.id = session->GetId();
	// packet.player.position = GRoom->Id2Player(session->GetId())->GetPosition();
	packet.player.position = player->GetPosition();
	buffer->CopyData(&packet, packet.header.size);

	return true;
}

bool MAKE_SC_REMOVE_PLAYER(shared_ptr<Session> session, shared_ptr<SendBuffer> buffer)
{
	SC_REMOVE_PLAYER_PACKET packet;
	packet.header = { sizeof(packet), SC_REMOVE_PLAYER };
	packet.playerId = session->GetId();
	buffer->CopyData(&packet, packet.header.size);
	return true;
}
