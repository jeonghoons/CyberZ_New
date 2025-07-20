#pragma once
#include "TestProtocol.h"
#include <functional>
#include "Session.h"

extern std::function<bool(shared_ptr<Session>&, BYTE*, int)> GPacketHandler[UINT16_MAX];

bool Handle_CS_LOGIN(shared_ptr<Session>& session, CS_LOGIN_PACKET& packet);
bool Handle_CS_CHAT(shared_ptr<Session>& session, CS_CHAT_PACKET& packet);


class PacketHandler
{
public:
	static void Init()
	{
		GPacketHandler[CS_LOGIN] = [](shared_ptr<Session>& session, BYTE* buffer, int len)
			{return HandlePacket<CS_LOGIN_PACKET>(Handle_CS_LOGIN, session, buffer, len); };
	}

	static bool HandlerPacket(shared_ptr<Session>& session, BYTE* buffer, int len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->type](session, buffer, len);
	}

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, shared_ptr<Session>& session, BYTE* buffer, int len)
	{
		PacketType pkt;



		return func(session, pkt);
	}
};


