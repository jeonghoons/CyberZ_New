#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(shared_ptr<Room> room) : _ownerRoom(room)
{
}
