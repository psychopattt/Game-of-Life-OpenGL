#include "InputStates.h"

InputStates::InputStates(int inputCount) : inputCount(inputCount)
{
	inputStates = std::make_unique<bool[]>(inputCount);
}

void InputStates::ResetInputs()
{
	if (enabledInputCount > 0)
	{
		std::fill(inputStates.get(), inputStates.get() + inputCount, false);
		enabledInputCount = 0;
	}
}

bool InputStates::Get(int input) const
{
	return inputStates[input];
}

void InputStates::Set(int input, bool state)
{
	if (inputStates[input] != state)
	{
		state ? enabledInputCount++ : enabledInputCount--;
		inputStates[input] = state;
	}
}

bool InputStates::Pop(int input)
{
	bool state = Get(input);
	Set(input, false);
	return state;
}
