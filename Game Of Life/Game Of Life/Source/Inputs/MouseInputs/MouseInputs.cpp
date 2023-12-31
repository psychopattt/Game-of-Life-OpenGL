#include "MouseInputs.h"

#include "Settings/TransformSettings/TransformSettings.h"
#include "Inputs/CurrentInputs/CurrentInputs.h"
#include "GLFW/glfw3.h"

using namespace CurrentInputs;
using namespace TransformSettings;

void MouseInputs::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	ScrolledUp = offsetY > 0;
	ScrolledDown = offsetY < 0;
}

void MouseInputs::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	
}

void MouseInputs::Update()
{
	UpdateZoom();
}

void MouseInputs::UpdateZoom()
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
