#include "ComputeBuffer.h"

unsigned int ComputeBuffer::GetId() const
{
	return id;
}

unsigned int ComputeBuffer::GetBindingId() const
{
	return bindingId;
}

void ComputeBuffer::Rebind(unsigned int bindingId)
{
	this->bindingId = bindingId;
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingId, id);
}

void ComputeBuffer::Destroy() const
{
	glDeleteBuffers(1, &id);
}
