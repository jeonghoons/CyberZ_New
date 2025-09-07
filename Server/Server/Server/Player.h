#pragma once
#include "Session.h"

class Player
{
public:
	Player() = default;
	Player(shared_ptr<Session> ownerSession);
	~Player() = default;

	shared_ptr<Session> GetSession() const { return _ownerSession.lock(); }
	void SetPosition(pair<int, int> position) { _position = position; }
	pair<int, int> GetPosition() const { return _position; }

private:
	weak_ptr<Session> _ownerSession;
	pair<int, int> _position = {};

	// map<int>		_viewList;
};



