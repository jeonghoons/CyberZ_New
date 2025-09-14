#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "PacketHandler.h"
#include "Player.h"
#include "Monster.h"

// shared_ptr<Room> GRoom = make_shared<Room>();
shared_ptr<RoomManager> GRoomManager = make_shared<RoomManager>();

#include "TimerQueue.h"

void Room::InitRoom()
{
	

	
	for (int i = 0; i < 40; ++i) {
		shared_ptr<Monster> monster = make_shared<Monster>();
		monster->SetId(MonsterIdGenerator());
		AddObject(monster);
	}
}



void Room::EnterRoom(shared_ptr<Player> player)
{
	
	player->SetOwnerRoom(shared_from_this());
	player->SetPosition(RandomPos());
	_players[player->GetId()] = player;


	SC_LOGIN_INFO_PACKET logInPacket;
	logInPacket.header = { sizeof(SC_LOGIN_INFO_PACKET), SC_LOGIN };
	logInPacket.player.id = player->GetId();
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


	int dataSize = sizeof(SC_ADD_OBJECT_PACKET) * _monsters.size();
	vector<BYTE> buf(dataSize);
	int bufIndex{};
	
	for (auto& [id, monster] : _monsters) {
		SC_ADD_OBJECT_PACKET addPacket;
		addPacket.header = { sizeof(addPacket), SC_ADD_OBJECT };
		addPacket.objectId = id;
		addPacket.position = monster->GetPosition();
		memcpy(&buf[bufIndex], &addPacket, sizeof(addPacket));
		bufIndex += sizeof(addPacket);
	}

	shared_ptr<SendBuffer> npcSendBuffer = make_shared<SendBuffer>(dataSize);
	npcSendBuffer->CopyData(buf.data(), dataSize);

	player->GetSession()->Send(npcSendBuffer);

	Update();
}

void Room::LeaveRoom(shared_ptr<Player> player)
{

	shared_ptr<SendBuffer> sendBuffer = PacketHandler::MakePacket(player->GetSession(), SC_PACKET_LIST::SC_REMOVE_PLAYER);
	Broadcast(sendBuffer);

	RWLock::WriteGuard lock(_lock);
	_players.erase(player->GetId());

}

void Room::Broadcast(shared_ptr<SendBuffer> sendBuffer)
{
	RWLock::ReadGuard lock(_lock);
	for (const auto& p : _players)
	{
		p.second->GetSession()->Send(sendBuffer);
	}
}

bool Room::AddObject(shared_ptr<Monster> object)
{
	RWLock::WriteGuard lock(_lock);
	_monsters.insert(make_pair(object->GetId(), object));
	object->SetOwnerRoom(shared_from_this());
	object->SetPosition(RandomPos());

	
	return true;
}

bool Room::RemoveObject(int objectId)
{
	return false;
}

void Room::Update()
{
	TimerEvent ev{ std::chrono::system_clock::now() + std::chrono::milliseconds(1000), EV_NPC_MOVE };
	GTimerQueue->_timerQueue.push(ev);
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

void Room::NPCMove()
{

}

pair<int, int> Room::RandomPos()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 32);

	int x = dist(gen);
	int y = dist(gen);

	return { x, y };
}

void RoomManager::CreateRoom()
{
	int id = IdGenerator();

	shared_ptr<Room> room = make_shared<Room>();
	_rooms.insert({ id, room });
	room->InitRoom();
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
	while (true) {
		{
			RWLock::WriteGuard lock(_lock);

			for (auto& [id, room] : _rooms) {
				if (room->NumPlayers() < MAX_ROOM_CAPACITY) {
					room->EnterRoom(player);
					return; // 성공 시 바로 종료
				}
			}

			// 빈 방이 없음 -> 새 방 생성
			CreateRoom();
		}

		// lock 해제된 상태에서 다시 while 재시도
		// (CreateRoom에서 _rooms에 새로운 방 추가됨)
		// 다음 루프에서 새로 생성된 방에 들어가게 됨
	}
}

int RoomManager::IdGenerator()
{
	static atomic<int> _idGenerator = 0;
	return ++_idGenerator;
}
