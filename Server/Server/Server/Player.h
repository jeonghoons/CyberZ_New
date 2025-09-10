#pragma once
#include "pch.h"
#include "Session.h"
#include "GameObject.h"
#include "Room.h"


class Player :public GameObject
{
public:
	Player() = default;
	Player(shared_ptr<Session> ownerSession);
	~Player() = default;

	shared_ptr<Session> GetSession() const { return _ownerSession.lock(); }
	void SetOwnerRoom(shared_ptr<Room> room) { _ownerRoom = room; }
	shared_ptr<Room> GetCurrentRoom() { return _ownerRoom.lock(); }

	void SetPosition(pair<int, int> position) { _position = position; }
	pair<int, int> GetPosition() const { return _position; }
	unsigned _last_moveTime{};

private:
	weak_ptr<Session> _ownerSession;
	pair<int, int> _position = {};

	weak_ptr<Room>	_ownerRoom;
	// map<int>		_viewList;
};



