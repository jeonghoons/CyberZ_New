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

	XMFLOAT3 RandomPos();

private:

	map<int, shared_ptr<Player>> _players;
};

extern shared_ptr<Room> GRoom;

