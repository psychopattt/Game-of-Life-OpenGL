#pragma once

#include "glad/glad.h"

class ComputeBuffer
{
	public:
		template <typename T>
		ComputeBuffer(const T data[], size_t size)
		{
			glGenBuffers(1, &id);
			bindingId = id;

			glBindBuffer(GL_SHADER_STORAGE_BUFFER, id);
			glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingId, id);
		}

		unsigned int GetId() const;
		unsigned int GetBindingId() const;
		void Rebind(unsigned int bindingId);
		void Destroy() const;

	private:
		unsigned int id;
		unsigned int bindingId;
};
