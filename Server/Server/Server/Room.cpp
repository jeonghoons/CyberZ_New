#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "Player.h"
#include "PacketHandler.h"

shared_ptr<Room> GRoom = make_shared<Room>();

void Room::EnterRoom(shared_ptr<Player> player)
{
	_players[player->GetSession()->GetId()] = player;	
	player->SetPosition(RandomPos());

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
	_players.erase(player->GetSession()->GetId());

	shared_ptr<SendBuffer> sendBuffer = PacketHandler::MakePacket(player->GetSession(), SC_PACKET_LIST::SC_DELETE_PLAYER);
	Broadcast(sendBuffer);
}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	for (auto& p : _players)
	{
		p.second->GetSession()->Send(sendBuffer);
	}
}

XMFLOAT3 Room::RandomPos()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1000.0f);

	float x = dist(gen);
	float y = dist(gen);
	float z = dist(gen);

	return XMFLOAT3(x, y, z);
}
