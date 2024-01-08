#include "ComputeBuffer.h"

unsigned int ComputeBuffer::GetId() const
{
	return id;
}

ComputeBuffer::~ComputeBuffer()
{
	glDeleteBuffers(1, &id);
}
