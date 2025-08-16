#include "pch.h"
#include "Session.h"
#include "ServerService.h"
#include "PacketHandler.h"

Session::Session() : _recvBuffer(65536)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

Session::~Session()
{
	if (_socket != INVALID_SOCKET)
		::closesocket(_socket);

	cout << "~Session" << endl;

	_socket = INVALID_SOCKET;
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int numBytes)
{
	switch (iocpEvent->type)
	{
	case EventType::Recv:
		ProcessRecv(numBytes);
		break;
	case EventType::Send:
		ProcessSend(numBytes);
		break;

	default:
		cout << "Undefined Event" << endl;
		break;
	}
}

void Session::OnConnected()
{
	_connected.store(true);
	cout << "Client[" << _cid << "] Connected " << endl;

	GetService()->AddSession(static_pointer_cast<Session>(shared_from_this()));
	
	RegisterRecv();
}

void Session::Disconnect(const WCHAR* cause)
{
	if (_connected.exchange(false) == false)
		return;

	GetService()->ReleaseSession(static_pointer_cast<Session>(shared_from_this()));

	wcout << "DisConnect :" << cause << endl;
}

void Session::Send(BYTE* buffer, int len)
{
	memcpy(_sendBuffer, buffer, len);
	RegisterSend();
}

int Session::ProcessData(BYTE* buffer, int len)
{
	int processLen = 0;

	while (true)
	{
		int dataSize = len - processLen;
		if (dataSize < sizeof(PacketHeader))
			break;

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processLen]));
		if (dataSize < header.size)
			break;

		// 패킷 조립 성공
		ProcessPacket(&buffer[processLen], header.size);

		processLen += header.size;

	}
	return processLen;
}

void Session::ProcessPacket(BYTE* buffer, int len)
{
	shared_ptr<Session> session = static_pointer_cast<Session>(shared_from_this());
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

	PacketHandler::HandlerPacket(session, buffer, len);

}

void Session::RegisterRecv()
{
	// thread safe
	if (IsConnected() == false)
		return;

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, &_recvEvent, nullptr))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}


}

void Session::ProcessRecv(int numOfBytes)
{
	_recvEvent.owner = nullptr;

	cout << "Recv " << numOfBytes << " Bytes" << endl;
	if (numOfBytes == 0) {
		Disconnect(L"Recv 0");
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false) {
		return;
	}

	int dataSize = _recvBuffer.DataSize();

	int processLen = ProcessData(_recvBuffer.ReadPos(), dataSize);
	if (_recvBuffer.OnRead(processLen) == false)
	{
		Disconnect(L"Recv Buffer Overflow");
		return;
	}

	_recvBuffer.CleanCheck();

	RegisterRecv();
}

void Session::RegisterSend()
{
	if (IsConnected() == false)
		return;

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();

	WSABUF wsaBuf;
	wsaBuf.buf = _sendBuffer;
	wsaBuf.len = sizeof(_sendBuffer);

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, &numOfBytes, 0, &_sendEvent, nullptr))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_sendEvent.owner = nullptr; // Release REF
		}
	}
}

void Session::ProcessSend(int numOfBytes)
{
	_sendEvent.owner = nullptr;
	ZeroMemory(_sendBuffer, sizeof(_sendBuffer));
	cout << "Send " << numOfBytes << " Bytes" << endl;

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}
}


