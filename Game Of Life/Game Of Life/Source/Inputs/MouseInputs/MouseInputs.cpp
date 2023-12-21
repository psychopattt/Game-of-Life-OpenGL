#include "MouseInputs.h"

#include "Inputs/CurrentInputs/CurrentInputs.h"
#include "Settings/Settings.h"
#include "GLFW/glfw3.h"

void MouseInputs::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	CurrentInputs::ScrolledUp = offsetY > 0;
	CurrentInputs::ScrolledDown = offsetY < 0;
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
	int zoomSpeed = CurrentInputs::FastModifierHeld ? 100 :
		CurrentInputs::SlowModifierHeld ? 1 : 10;

	if (CurrentInputs::ScrolledUp)
	{
		CurrentInputs::ScrolledUp = false;
		Settings::CurrentZoom += zoomSpeed;

		if (Settings::CurrentZoom > 2000000000)
			Settings::CurrentZoom = 2000000000;
	}

	if (CurrentInputs::ScrolledDown)
	{
		CurrentInputs::ScrolledDown = false;
		Settings::CurrentZoom -= zoomSpeed;

		if (Settings::CurrentZoom < 0)
			Settings::CurrentZoom = 0;
	}
}
