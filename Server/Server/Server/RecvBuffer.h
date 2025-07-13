#pragma once
#include "pch.h"

class RecvBuffer
{
public:
	RecvBuffer(int bufferSize);
	~RecvBuffer();

	
	BYTE* ReadPos() { return &_buffer[_readPos]; }
	BYTE* WritePos() { return &_buffer[_writePos]; }
	int BufferSize() { return _buffer.size(); }
	
private:
	int		_readPos = 0;
	int		_writePos = 0;
	vector<BYTE>	 _buffer;
};