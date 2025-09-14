#include "pch.h"
#include "Player.h"

Player::Player(shared_ptr<Session> ownerSession) : _ownerSession(ownerSession)
{

}

Player::~Player()
{
	cout << "~Player[" << GetId() << "]" << endl;
	// _ownerRoom.reset();
	// _ownerSession.reset();
}
