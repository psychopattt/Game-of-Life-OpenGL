#include "MouseHandler.h"

#include "GLFW/glfw3.h"

#include "Inputs/InputStates/InputStates.h"
#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

using namespace TransformSettings;

MouseHandler::MouseHandler() : DeviceHandler(GLFW_MOUSE_BUTTON_LAST) { }

void MouseHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	ApplyZoom(offsetY > 0, offsetY < 0);
}

void MouseHandler::ApplyZoom(bool scrolledUp, bool scrolledDown)
{
	if (scrolledUp)
	{
		Zoom += SpeedMultiplier;

		if (Zoom > MaxZoom)
			Zoom = MaxZoom;
	}

	if (scrolledDown)
	{
		if (Zoom > SpeedMultiplier)
			Zoom -= SpeedMultiplier;
		else
			Zoom = 0;
	}
}

void MouseHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	inputStates->Set(button, action != GLFW_RELEASE);
}

void MouseHandler::Update(double deltaTime)
{
	ApplyMousePan();
}

void MouseHandler::ApplyMousePan()
{
	if (inputStates->Get(GLFW_MOUSE_BUTTON_LEFT) && !MousePanEnabled)
		Settings::Gui->GetMousePosition(MousePanStartX, MousePanStartY);

	MousePanEnabled = inputStates->Get(GLFW_MOUSE_BUTTON_LEFT);
}
