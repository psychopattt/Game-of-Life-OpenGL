#include "MouseHandler.h"

#include "GLFW/glfw3.h"

#include "Inputs/CurrentInputs.h"
#include "Settings/TransformSettings.h"

using namespace CurrentInputs;
using namespace TransformSettings;

void MouseHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	ScrolledUp = offsetY > 0;
	ScrolledDown = offsetY < 0;
}

void MouseHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	switch (button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:
			MousePanEnabled = action == GLFW_PRESS;
			glfwGetCursorPos(window, &MousePanStartX, &MousePanStartY);
			break;
	}
}

void MouseHandler::Update()
{
	UpdateZoom();
}

void MouseHandler::UpdateZoom()
{
	if (ScrolledUp)
	{
		ScrolledUp = false;
		Zoom += SpeedMultiplier;

		if (Zoom > MaxZoom)
			Zoom = MaxZoom;
	}

	if (ScrolledDown)
	{
		ScrolledDown = false;

		if (Zoom > SpeedMultiplier)
			Zoom -= SpeedMultiplier;
		else
			Zoom = 0;
	}
}
