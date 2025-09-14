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
	virtual ~Player() = default;

	shared_ptr<Session> GetSession()
	{
		auto s = _ownerSession.lock();
		if (s) {
			/*std::cout << "[Warning] Player::GetSession(): session expired!" << std::endl;
			OutputDebugStringA("[Warning] Player::GetSession(): session expired!\n");
			__debugbreak();*/
			return s;
		}
		else
			return nullptr;
	}
	void SetOwnerSession(shared_ptr<Session> session) { _ownerSession = session; }
	
	

private:
	weak_ptr<Session> _ownerSession;
	
	// map<int>		_viewList;
};



