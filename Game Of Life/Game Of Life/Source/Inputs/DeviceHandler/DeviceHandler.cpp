#include "DeviceHandler.h"

#include "Inputs/InputStates/InputStates.h"

DeviceHandler::DeviceHandler(int inputCount)
{
	inputStates = std::make_unique<InputStates>(inputCount);
}

InputStates& DeviceHandler::GetInputStates()
{
	return *inputStates;
}

DeviceHandler::~DeviceHandler() { }
