#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "Player.h"
#include "PacketHandler.h"

// shared_ptr<Room> GRoom = make_shared<Room>();
shared_ptr<RoomManager> GRoomManager = make_shared<RoomManager>();

void Room::InitRoom()
{
	/*_tileMap.reserve(400);
	for (int i = 0; i < _tileMap.size(); ++i) {
		for (int j = 0; j < _tileMap.size(); ++j) {
			_tileMap[i].push_back(true);
			_tileMap[i]
		}
	}*/

	for (int i = 0; i < 100; ++i) {
		shared_ptr<GameObject> monster = make_shared<GameObject>(shared_from_this());
		monster->SetPosition(RandomPos());
		_objects[i] = monster;
	}
}



void Room::EnterRoom(shared_ptr<Player> player)
{
	int p_id = player->GetSession()->GetId();
	_players[p_id] = player;	

	player->SetOwnerRoom(shared_from_this());

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

void Room::Update()
{

}

void Room::PlayerMove(shared_ptr<Player> player, int direction, unsigned move_time)
{
	pair<int, int> pos = player->GetPosition();
	player->_last_moveTime = move_time;

	switch (direction)
	{
	case 2: // left
		pos.first -= 1;
		break;
	case 3: // right
		pos.first += 1;
		break;
	case 0: // up
		pos.second -= 1;
		break;
	case 1: // down
		pos.second += 1;
		break;

	default:
		cout << "Move Error" << endl;
		break;
	}

	player->SetPosition(pos);

	SC_MOVE_PACKET packet;
	packet.header = { sizeof(packet), SC_MOVE_OBJECT };
	packet.id = player->GetSession()->GetId();
	packet.position = player->GetPosition();
	packet.move_time = player->_last_moveTime;

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(sizeof(packet));
	sendBuffer->CopyData(&packet, sizeof(packet));
	Broadcast(sendBuffer);

}

void Room::RandomMove()
{

}

pair<int, int> Room::RandomPos()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 4);

	int x = dist(gen);
	int y = dist(gen);

	return { x, y };
}

void RoomManager::CreateRoom()
{
	int id = IdGenerator();
	
	shared_ptr<Room> room = make_shared<Room>();
	_rooms.insert({ id, room });
}

void RoomManager::Remove(int roomId)
{
	RWLock::WriteGuard lock(_lock);
	_rooms.erase(roomId);
}

void RoomManager::Remove(shared_ptr<Room> room)
{
	return;
}

void RoomManager::EnterPlayer(shared_ptr<Player> player)
{

	for (auto it = _rooms.begin(); it != _rooms.end(); ++it) {
		if(it->second->NumPlayers() < 2) {
			RWLock::WriteGuard lock(_lock);
			it->second->EnterRoom(player);
			break;
		}
	}

	if (player->GetCurrentRoom() == nullptr) {
		CreateRoom();
		EnterPlayer(player);
	}


}

int RoomManager::IdGenerator()
{
	static atomic<int> _idGenerator = 0;
	return ++_idGenerator;
}
