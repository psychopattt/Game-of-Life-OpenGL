#include "MouseHandler.h"

#include "Inputs/CurrentInputs.h"
#include "Settings/TransformSettings.h"
#include "GLFW/glfw3.h"

using namespace CurrentInputs;
using namespace TransformSettings;

void MouseHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	ScrolledUp = offsetY > 0;
	ScrolledDown = offsetY < 0;
}

void MouseHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	
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
