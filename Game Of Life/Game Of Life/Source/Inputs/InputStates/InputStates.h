#pragma once

#include <memory>

class InputStates
{
	public:
		InputStates(int inputCount);
		void ResetInputs();
		bool Get(int input) const;
		void Set(int input, bool state);
		bool Pop(int input);

	private:
		const int inputCount;
		int enabledInputCount = 0;
		std::unique_ptr<bool[]> inputStates;
};
