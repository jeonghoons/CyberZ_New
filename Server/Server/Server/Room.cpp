#include "pch.h"
#include "Room.h"
#include "Session.h"
#include "PacketHandler.h"
#include "Player.h"
#include "Monster.h"

// shared_ptr<Room> GRoom = make_shared<Room>();
shared_ptr<RoomManager> GRoomManager = make_shared<RoomManager>();

#include "TimerQueue.h"

HANDLE Room::GetHandle()
{
	return HANDLE();
}

void Room::Dispatch(IocpEvent* iocpEvent, int numBytes)
{
	switch (iocpEvent->type)
	{
	case EventType::RoomUpdate:
		cout << "RoomUpdate - " << endl;
		Update();
		break;

	case EventType::NPC_MOVE:
		cout << "NPC_MOVE" << endl;

		break;
	

	default:
		cout << "Undefined Event" << endl;
		break;
	}

	delete iocpEvent;
}

void Room::InitRoom()
{
		
	/*for (int i = 0; i < 40; ++i) {
		shared_ptr<Monster> monster = make_shared<Monster>();
		monster->SetId(MonsterIdGenerator());
		AddObject(monster);
	}

	Update();*/
}



void Room::EnterRoom(shared_ptr<Player> player)
{
	

	player->SetOwnerRoom(static_pointer_cast<Room>(shared_from_this()));
	player->SetPosition(RandomPos());
	_players[player->GetId()] = player;


	SC_LOGIN_INFO_PACKET logInPacket;
	logInPacket.header = { sizeof(SC_LOGIN_INFO_PACKET), SC_LOGIN };
	logInPacket.player.id = player->GetId();
	logInPacket.player.position = player->GetPosition();

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(sizeof(logInPacket));
	sendBuffer->CopyData(&logInPacket, sizeof(logInPacket));
	player->GetSession()->Send(sendBuffer);


	{ // ���� �÷��̾� �þ�ó��
		// RWLock::ReadGuard vlock(_lock);
		// auto players = _players;
		for (auto& [id, oldplayer] : _players) {
			if (player->GetId() == id) continue;
			if (false == canSee(player->GetId(), id)) continue;

			{
				RWLock::WriteGuard lock(_lock);
				_players[id]->_viewList.insert(player->GetId());
				oldplayer->GetSession()->Send(PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_ADD_PLAYER));
				
				player->_viewList.insert(id);
				player->GetSession()->Send(PacketHandler::MakePacket(oldplayer, SC_PACKET_LIST::SC_ADD_PLAYER));
			}

		}
	}


	/*int dataSize = sizeof(SC_ADD_OBJECT_PACKET) * _monsters.size();
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

	player->GetSession()->Send(npcSendBuffer);*/

	
}

void Room::LeaveRoom(shared_ptr<Player> player)
{

	shared_ptr<SendBuffer> sendBuffer = PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_REMOVE_PLAYER);
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
	object->SetOwnerRoom(static_pointer_cast<Room>(shared_from_this()));
	object->SetPosition(RandomPos());

	
	return true;
}

bool Room::RemoveObject(int objectId)
{
	return false;
}

void Room::Update()
{
	TimerEvent ev{ std::chrono::system_clock::now() + std::chrono::milliseconds(1000), EV_UPDATE_ROOM, static_pointer_cast<Room>(shared_from_this())};
	GTimerQueue->_timerQueue.push(ev);
	NPCMove();
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


	unordered_set<int> near_list;
	unordered_set<int> old_list;
	map<int, shared_ptr<Player>> players;
	
	{
		RWLock::ReadGuard copylock(_lock);
		old_list = player->_viewList;
		players = _players;
	}

	for (auto& [id, pl] : players) { // ������ �þ� ����
		if (id == player->GetId()) continue;
		if (canSee(player->GetId(), id))
			near_list.insert(id);
	}

	player->GetSession()->Send(PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_MOVE_OBJECT));

	for (int id : near_list) {
		if (_players[id]->_viewList.count(player->GetId())) // ��뿡 ���������� move
			_players[id]->GetSession()->Send(PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_MOVE_OBJECT));
		else {// ������ add
			{
				RWLock::WriteGuard lock(_lock);
				_players[id]->_viewList.insert(player->GetId());
			}
			_players[id]->GetSession()->Send(PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_ADD_PLAYER));
		}
		if (old_list.count(id) == 0) {// �� �þ߿� ������ ������ ������ move
			{
				RWLock::WriteGuard lock(_lock);
				player->_viewList.insert(id);
			}
			player->GetSession()->Send(PacketHandler::MakePacket(_players[id], SC_PACKET_LIST::SC_ADD_PLAYER));
		}
	}

	for (int id : old_list) {
		if (near_list.count(id) == 0) { // ����þ߿��� ���þ߻���� ������ ���� remove
			
			RWLock::WriteGuard lock(_lock);
			player->_viewList.erase(id);
			player->GetSession()->Send(PacketHandler::MakePacket(_players[id], SC_PACKET_LIST::SC_REMOVE_PLAYER));

			_players[id]->_viewList.erase(player->GetId());
			_players[id]->GetSession()->Send(PacketHandler::MakePacket(player, SC_PACKET_LIST::SC_REMOVE_PLAYER));

		}
	}

	
	
}

void Room::NPCMove()
{

	auto monsters = _monsters;

	for (auto monster : monsters) {
		
		int x = monster.second->GetPosition().first;
		int y = monster.second->GetPosition().second;
		switch (rand() % 4) {
		case 0:
			if (x < (16 - 1)) x++; break;
		case 1:
			if (x > 0) x--; break;
		case 2:
			if (y < (16 - 1)) y++; break;
		case 3:
			if (y > 0) y--; break;
		}
		monster.second->SetPosition({ x, y });
	}
		

	int dataSize = sizeof(SC_MOVE_PACKET) * _monsters.size();
	vector<BYTE> buf(dataSize);
	int bufIndex{};

	for (auto& [id, monster] : _monsters) {
		SC_MOVE_PACKET movePacket;
		movePacket.header = { sizeof(movePacket), SC_MOVE_OBJECT };
		movePacket.id = id;
		movePacket.position = monster->GetPosition();
		memcpy(&buf[bufIndex], &movePacket, sizeof(movePacket));
		bufIndex += sizeof(movePacket);
	}

	shared_ptr<SendBuffer> sendBuffer = make_shared<SendBuffer>(bufIndex);
	Broadcast(sendBuffer);

}

bool Room::canSee(int from, int to)
{
	if (abs(_players[from]->GetPosition().first - _players[to]->GetPosition().first) > 5) return false;
	return abs(_players[from]->GetPosition().second - _players[to]->GetPosition().second) <= 5;
}

pair<int, int> Room::RandomPos()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> dist(0, 31);

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
					return; // ���� �� �ٷ� ����
				}
			}

			// �� ���� ���� -> �� �� ����
			CreateRoom();
		}

		// lock ������ ���¿��� �ٽ� while ��õ�
		// (CreateRoom���� _rooms�� ���ο� �� �߰���)
		// ���� �������� ���� ������ �濡 ���� ��
	}
}

int RoomManager::IdGenerator()
{
	static atomic<int> _idGenerator = 0;
	return ++_idGenerator;
}
