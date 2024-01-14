#pragma once

#include <memory>

class InputStates;

class DeviceHandler
{
	public:
		virtual InputStates& GetInputStates();
		virtual void Update(double deltaTime) = 0;

	protected:
		DeviceHandler(int inputCount);
		~DeviceHandler();

		std::unique_ptr<InputStates> inputStates;
};
