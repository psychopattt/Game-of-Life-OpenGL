#pragma once

#include <memory>
#include "../ComputeBuffer/ComputeBuffer.h"

using std::unique_ptr, std::make_unique;

class DualComputeBuffer
{
	public:
		DualComputeBuffer(size_t sizeBytes);
		DualComputeBuffer(const void* data, size_t sizeBytes);
		unsigned int GetId(int bufferIndex);
		void Swap();
		~DualComputeBuffer();

	private:
		bool swapped = false;
		unique_ptr<ComputeBuffer> buffer1;
		unique_ptr<ComputeBuffer> buffer2;
};
