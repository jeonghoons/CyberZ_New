#pragma once
#include "pch.h"
#include "RecvBuffer.h"


class Network
{
public:
	Network();
	~Network();

	bool Connect2Server();

	void Send_packet(void* buffer)
	{
		unsigned char* p = reinterpret_cast<unsigned char*>(buffer);
		size_t sent = 0;
		// int len = send(_socket, reinterpret_cast<char*>(p), static_cast<int>(p[0]), sent);
		_socket.send(buffer, p[0], sent);
		cout << sent << "Bytes Send" << endl;
	}

	int RecvPacket()
	{

		size_t received = 0;

		// int len = recv(_socket, reinterpret_cast<char*>(_recvBuffer.WritePos()), _recvBuffer.FreeSize(), sent);
		auto recv_result = _socket.receive(_recvBuffer.WritePos(), _recvBuffer.FreeSize(), received);
		if (recv_result == sf::Socket::Error)
		{
			wcout << L"Recv 에러!";
			exit(-1);
		}
		if (recv_result == sf::Socket::Disconnected) {
			wcout << L"Disconnected\n";
			exit(-1);
		}
		
		if (recv_result != sf::Socket::NotReady)
			if (received > 0) process_data(_recvBuffer.ReadPos(), received);

		//if (received > 0) {
		//	process_data(_recvBuffer.ReadPos(), received);
		//	// cout << len << "Bytes Recv" << endl;
		//}
		
		return received;
	}

	int process_data(BYTE* net_buf, int io_byte)
	{
		int processLen = 0;

		while (true)
		{
			int dataSize = io_byte - processLen;
			if (dataSize < sizeof(PacketHeader))
				break;

			PacketHeader header = *(reinterpret_cast<PacketHeader*>(&net_buf[processLen]));
			if (dataSize < header.size)
				break;

			// 패킷 조립 성공
			ProcessPacket(&net_buf[processLen], header.size);

			processLen += header.size;

		}
		return processLen;

	}

	void ProcessPacket(BYTE* net_buf, int io_byte)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(net_buf);

		switch (header->type)
		{
		case SC_PACKET_LIST::SC_ADD_PLAYER:
			cout << "ADD PLAYER " << io_byte << "Bytes " << endl;
			
			break;

		case SC_PACKET_LIST::SC_DELETE_PLAYER:
			cout << "DELETE PLAYER " << io_byte << "Bytes " << endl;
			break;

		case SC_PACKET_LIST::SC_CHAT:
			cout << "Chatting " << io_byte << "Bytes " << endl;
			break;

		default:
			break;
		}
	}

private:
	sf::TcpSocket		_socket;
	RecvBuffer	_recvBuffer;
	char			_sendBuffer[1024] = { 0, };


};

