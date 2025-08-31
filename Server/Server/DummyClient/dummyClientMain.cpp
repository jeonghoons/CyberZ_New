
#include "pch.h"
#include "Network.h"
#include <string>

//void HandleError(const char* cause)
//{
//	int errCode = ::WSAGetLastError();
//	cout << cause << " ErrorCode : " << errCode << endl;
//}

static const int   TARGET_FPS = 60;           // 목표 FPS

class Player
{
public:
	Player() = default;
	~Player() = default;
};

map<int, Player> players;

int main()
{

	Network network;
	if (network.Connect2Server() == false)
		exit(-1);

	CS_LOGIN_PACKET loginPkt;
	loginPkt.header.type = CS_PACKET_LIST::CS_LOGIN;
	loginPkt.header.size = sizeof(loginPkt);
	network.Send_packet(&loginPkt);
	
	cout << "Connected to Server" << endl;

	std::thread inputThread([&network]() {
		while (true)
		{
			std::string input;
			std::getline(std::cin, input);
			if (input.empty()) continue;

			CS_CHAT_PACKET packet;
			strncpy_s(packet.message, input.c_str(), sizeof(packet.message) - 1);
			packet.message[sizeof(packet.message) - 1] = '\0';  // 널 종료 보장
			packet.header.size = static_cast<uint16_t>(std::strlen(packet.message) + sizeof(PacketHeader));
			packet.header.type = CS_CHAT;
			network.Send_packet(&packet);

		}
		});

	// 4) 메인 루프 (60 FPS) ------------------------------------------------
	using clock = std::chrono::high_resolution_clock;
	using duration = clock::duration;
	auto nextFrame = clock::now();
	static const duration FRAME_TIME = std::chrono::duration_cast<duration>(std::chrono::duration<double>(1.0 / TARGET_FPS));


	while (true)
	{
		const auto frameStart = clock::now();


		// 프레임 유지
		while (clock::now() - frameStart < FRAME_TIME) {
			// 실제 게임 클라이언트와 유사하게 sleep 없이 루프를 유지합니다.
		}

		nextFrame = frameStart;

		// FPS 출력 (1초마다)
		static int fpsCounter = 0;
		++fpsCounter;
		static auto lastFpsReport = frameStart;
		if (frameStart - lastFpsReport >= std::chrono::seconds(1))
		{
			network.RecvPacket();
			// std::cout << "FPS: " << fpsCounter << "\n";
			fpsCounter = 0;
			lastFpsReport = frameStart;
		}
	}

	inputThread.detach();  // 입력 스레드 분리 종료
	
	return 0;
}