#include "DualComputeBuffer.h"

#include "../ComputeBuffer/ComputeBuffer.h"

DualComputeBuffer::DualComputeBuffer(size_t sizeBytes) :
	DualComputeBuffer(nullptr, sizeBytes) { }

DualComputeBuffer::DualComputeBuffer(const void* data, size_t sizeBytes)
{
	buffer1 = std::make_unique<ComputeBuffer>(data, sizeBytes);
	buffer2 = std::make_unique<ComputeBuffer>(data, sizeBytes);
}

unsigned int DualComputeBuffer::GetId(int bufferIndex)
{
	if (!swapped)
	{
		if (bufferIndex == 0)
			return buffer1->GetId();
		else
			return buffer2->GetId();
	}
	else
	{
		if (bufferIndex == 0)
			return buffer2->GetId();
		else
			return buffer1->GetId();
	}
}

void DualComputeBuffer::Swap()
{
	swapped = !swapped;
}

DualComputeBuffer::~DualComputeBuffer() { }
