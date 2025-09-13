#pragma once
#include "pch.h"
#include "Room.h"

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	GameObject(shared_ptr<Room> room);

public:
	shared_ptr<Room> GetCurrentRoom() const { return _ownerRoom.lock(); }
	void SetOwnerRoom(shared_ptr<Room> room) { _ownerRoom = room; }

	pair<int, int> GetPosition() const { return _position; }
	void SetPosition(pair<int, int> position) { _position = position; }

	int GetId() const { return _oId; }
	void SetId(int id) { _oId = id; }

	unsigned _last_moveTime{};

protected:
	int				_oId{};
	pair<int, int> _position = {};
	weak_ptr<Room> _ownerRoom;
};

