#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "Player.h"
#include "PacketHandler.h"

shared_ptr<Room> GRoom = make_shared<Room>();

void Room::EnterRoom(shared_ptr<Player> player)
{
	int p_id = player->GetSession()->GetId();
	_players[p_id] = player;	
	player->SetPosition(RandomPos());

	SC_LOGIN_INFO_PACKET logInPacket;
	logInPacket.header = { sizeof(SC_LOGIN_INFO_PACKET), SC_LOGIN };
	logInPacket.player.id = p_id;
	logInPacket.player.position = player->GetPosition();

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(sizeof(logInPacket));
	sendBuffer->CopyData(&logInPacket, sizeof(logInPacket));
	player->GetSession()->Send(sendBuffer);


	// 신규에게 기존유저들을 ADD
	for (auto& p : _players) {
		if (p.second == player)
			continue;
		player->GetSession()->Send(PacketHandler::MakePacket(p.second->GetSession(), SC_PACKET_LIST::SC_ADD_PLAYER));
	}

	// 기존유저들에게 신규유저를 ADD
	for (auto& p : _players) {
		if (p.second == player)
			continue;
		p.second->GetSession()->Send(PacketHandler::MakePacket(player->GetSession(), SC_PACKET_LIST::SC_ADD_PLAYER));
	}
	

}

void Room::LeaveRoom(shared_ptr<Player> player)
{

	shared_ptr<SendBuffer> sendBuffer = PacketHandler::MakePacket(player->GetSession(), SC_PACKET_LIST::SC_REMOVE_PLAYER);
	Broadcast(sendBuffer);

	_players.erase(player->GetSession()->GetId());
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto& p : _players)
	{
		p.second->GetSession()->Send(sendBuffer);
	}
}

pair<int, int> Room::RandomPos()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 10);

	int x = dist(gen);
	int y = dist(gen);

	return { x, y };
}
