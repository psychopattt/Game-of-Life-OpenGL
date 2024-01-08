#include "DualComputeBuffer.h"

void DualComputeBuffer::Swap()
{
	swapped = !swapped;
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

DualComputeBuffer::~DualComputeBuffer() { }
