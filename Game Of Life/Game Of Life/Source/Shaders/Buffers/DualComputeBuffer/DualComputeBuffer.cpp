#include "DualComputeBuffer.h"

void DualComputeBuffer::Swap()
{
	const unsigned int bindingId1 = buffer1->GetBindingId();
	buffer1->Rebind(buffer2->GetBindingId());
	buffer2->Rebind(bindingId1);
}

void DualComputeBuffer::Destroy()
{
	buffer1->Destroy();
	delete buffer1;

	buffer2->Destroy();
	delete buffer2;
}
