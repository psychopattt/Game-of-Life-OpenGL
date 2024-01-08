#pragma once

#include "glad/glad.h"

class ComputeBuffer
{
	public:
		template <typename T>
		ComputeBuffer(const T data[], size_t size)
		{
			glGenBuffers(1, &id);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, id, id);
		}

		unsigned int GetId() const;
		~ComputeBuffer();

	private:
		unsigned int id;
};
