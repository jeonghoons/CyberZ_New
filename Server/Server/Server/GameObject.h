#pragma once
#include "pch.h"
#include "Room.h"

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	GameObject(shared_ptr<Room> room);


	shared_ptr<Room> GetRoom() { return _ownerRoom.lock(); }
	pair<int, int> GetPosition() const { return _position; }
	void SetPosition(pair<int, int> position) { _position = position; }



private:
	pair<int, int> _position = {};
	weak_ptr<Room> _ownerRoom;
};

