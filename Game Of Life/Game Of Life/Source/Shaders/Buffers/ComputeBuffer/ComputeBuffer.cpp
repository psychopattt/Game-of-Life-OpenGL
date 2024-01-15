#include "ComputeBuffer.h"

#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"

ComputeBuffer::ComputeBuffer(size_t sizeBytes) :
	ComputeBuffer(nullptr, sizeBytes) { }

ComputeBuffer::ComputeBuffer(const void* data, size_t sizeBytes)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeBytes, data, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, id, id);

	LogGenerationFailure(sizeBytes);
}

unsigned int ComputeBuffer::GetId() const
{
	return id;
}

ComputeBuffer::~ComputeBuffer()
{
	glDeleteBuffers(1, &id);
}

void ComputeBuffer::LogGenerationFailure(size_t sizeBytes) const
{
	int errorCode = glGetError();

	if (errorCode != GL_NO_ERROR)
	{
		size_t sizeGb = sizeBytes / 1000000000;

		Settings::Log << "Buffer Error - Failed to generate buffer\nId: " <<
			id << ", size: " << sizeBytes << " bytes (~" << sizeGb <<
			"GB), error code: " << errorCode << "\n\n";
	}
}
