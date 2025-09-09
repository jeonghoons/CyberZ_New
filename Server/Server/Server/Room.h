#pragma once

class Session;
class SendBuffer;
class Player;

class Room
{
public:
	void EnterRoom(shared_ptr<Player> player);
	void LeaveRoom(shared_ptr<Player> player);
	void Broadcast(shared_ptr<SendBuffer> sendBuffer);

	void PlayerMove(shared_ptr<Player> player, int direction);

	shared_ptr<Player> Id2Player(int pId) { return _players[pId]; }

	pair<int, int> RandomPos();

private:

	map<int, shared_ptr<Player>> _players;
};

extern shared_ptr<Room> GRoom;

