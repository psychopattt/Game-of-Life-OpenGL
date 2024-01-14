#pragma once

#include "Inputs/DeviceHandler/DeviceHandler.h"

struct GLFWwindow;

class MouseHandler : DeviceHandler
{
	public:
		MouseHandler();
		void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY);
		void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
		void Update(double deltaTime);

	private:
		void ApplyZoom(bool scrolledUp, bool scrolledDown);
		void ApplyMousePan();
};
