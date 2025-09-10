#pragma once
#include "pch.h"

class Session;
class SendBuffer;
class GameObject;
class Player;

class Room : public enable_shared_from_this<Room>
{
public:
	void InitRoom();
	
	void EnterRoom(shared_ptr<Player> player);
	void LeaveRoom(shared_ptr<Player> player);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);


	void Update();
	void PlayerMove(shared_ptr<Player> player, int direction, unsigned move_time);
	void RandomMove();

	shared_ptr<Player> Id2Player(int pId) { return _players[pId]; }

	pair<int, int> RandomPos();

	int NumPlayers() { return _players.size(); }

private:
	/// <vector<bool>> _tileMap;
	map<int, shared_ptr<GameObject>> _objects;

	map<int, shared_ptr<Player>> _players;

	
};

// extern shared_ptr<Room> GRoom;

class RoomManager
{
public:
	void CreateRoom();
	void Remove(int roomId);
	void Remove(shared_ptr<Room> room);

	void EnterPlayer(shared_ptr<Player> player);

	int IdGenerator();

public:
	
	RWLock				_lock;
	unordered_map<int, shared_ptr<Room>> _rooms;
};

extern shared_ptr<RoomManager> GRoomManager;
