#pragma once
#include "Session.h"

class Player
{
public:
	Player() = default;
	Player(shared_ptr<Session> ownerSession);
	~Player() = default;

	shared_ptr<Session> GetSession() { return _ownerSession.lock(); }
	void SetPosition(XMFLOAT3 position) { _position = position; }

private:
	weak_ptr<Session> _ownerSession;
	XMFLOAT3 _position;
};

