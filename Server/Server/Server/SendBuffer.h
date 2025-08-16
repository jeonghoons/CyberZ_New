#pragma once

class SendBuffer
{
public:
	SendBuffer(int bufferSize);
	~SendBuffer();

	BYTE* Buffer() { return _buffer.data(); }
	int WritePos() { return _writePos; }
	int Capacity() { return static_cast<int>(_buffer.size()); }

	void CopyData(void* data, int len);

private:
	vector<BYTE> _buffer;
	int		_writePos = 0;
};

