#pragma once

#include "../ComputeBuffer/ComputeBuffer.h"

class DualComputeBuffer
{
	public:
		template <typename T>
		DualComputeBuffer(const T data[], size_t size)
		{
			buffer1 = new ComputeBuffer(data, size);
			buffer2 = new ComputeBuffer(data, size);
		}

		void Swap();
		~DualComputeBuffer();

	private:
		ComputeBuffer* buffer1;
		ComputeBuffer* buffer2;
};
