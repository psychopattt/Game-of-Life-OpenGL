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
		~DualComputeBuffer();

	private:
		unique_ptr<ComputeBuffer> buffer1;
		unique_ptr<ComputeBuffer> buffer2;
};
