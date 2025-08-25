#pragma once
#include "../Server/RecvBuffer.h"

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
		int len = send(_socket, reinterpret_cast<char*>(p), static_cast<int>(p[0]), sent);
		cout << len << "Bytes Send" << endl;
	}

	int RecvPacket()
	{

		size_t sent = 0;

		int len = recv(_socket, reinterpret_cast<char*>(_recvBuffer.WritePos()), _recvBuffer.FreeSize(), sent);

		
		if (len > 0) {
			process_data(_recvBuffer.ReadPos(), len);
			// cout << len << "Bytes Recv" << endl;
		}
		
		return len;
	}

	int process_data(BYTE* net_buf, int io_byte)
	{
		/*char* ptr = net_buf;
		static size_t in_packet_size = 0;
		static size_t saved_packet_size = 0;
		static char packet_buffer[BUF_SIZE];

		while (0 != io_byte) {
			if (0 == in_packet_size) in_packet_size = ptr[0];
			if (io_byte + saved_packet_size >= in_packet_size) {
				memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
				ProcessPacket(packet_buffer);
				ptr += in_packet_size - saved_packet_size;
				io_byte -= in_packet_size - saved_packet_size;
				in_packet_size = 0;
				saved_packet_size = 0;
			}
			else {
				memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
				saved_packet_size += io_byte;
				io_byte = 0;
			}
		}*/

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
		case SC_PACKET_LIST::SC_LOGIN:
			cout << "ADD PLAYER " << io_byte << "Bytes " << endl;
			break;

		case SC_PACKET_LIST::SC_CHAT:
			cout << "Chatting " << io_byte << "Bytes " << endl;
			break;

		default:
			break;
		}
	}

private:
	SOCKET		_socket;
	RecvBuffer	_recvBuffer;
	char			_sendBuffer[1024] = { 0, };


};

