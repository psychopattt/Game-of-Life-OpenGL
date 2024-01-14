#pragma once

#include "Inputs/DeviceHandler/DeviceHandler.h"

class KeyboardHandler : DeviceHandler
{
	public:
		KeyboardHandler();
		void HandleKeyboard(struct GLFWwindow* window,
			int key, int scanCode, int action, int mods);
		void Update(double deltaTime);

	private:
		void ReleaseCapturedKeys();
		void ApplySpeedMultiplier();
		void ApplyPan(double deltaTime);
		void ApplyFrameStep();
};
