#include "DualComputeBuffer.h"

void DualComputeBuffer::Swap()
{
	const unsigned int bindingId1 = buffer1->GetBindingId();
	buffer1->Rebind(buffer2->GetBindingId());
	buffer2->Rebind(bindingId1);
}

DualComputeBuffer::~DualComputeBuffer() { }
