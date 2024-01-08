#pragma once

#include <memory>
#include "../ComputeBuffer/ComputeBuffer.h"

using std::unique_ptr, std::make_unique;

class DualComputeBuffer
{
	public:
		template <typename T>
		DualComputeBuffer(const T data[], size_t size)
		{
			buffer1 = make_unique<ComputeBuffer>(data, size);
			buffer2 = make_unique<ComputeBuffer>(data, size);
		}

		void Swap();
		unsigned int GetId(int bufferIndex);
		~DualComputeBuffer();

	private:
		bool swapped = false;
		unique_ptr<ComputeBuffer> buffer1;
		unique_ptr<ComputeBuffer> buffer2;
};
