#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int bufferSize)
{
	_buffer.resize(bufferSize);
}

RecvBuffer::~RecvBuffer()
{

}


