#pragma once

#include "glad/gl.h"

class ComputeBuffer
{
	public:
		ComputeBuffer(size_t sizeBytes);
		ComputeBuffer(const void* data, size_t sizeBytes);
		unsigned int GetId() const;
		~ComputeBuffer();

	private:
		unsigned int id;

		void LogGenerationFailure(size_t sizeBytes) const;
};
