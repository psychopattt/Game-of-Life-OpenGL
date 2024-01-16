#include "MouseHandler.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

using Settings::Gui;
using namespace TransformSettings;

void MouseHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	if (!ImGui::GetIO().WantCaptureMouse)
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
	else if (scrolledDown)
	{
		if (Zoom > SpeedMultiplier)
			Zoom -= SpeedMultiplier;
		else
			Zoom = 0;
	}
}

void MouseHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	
}

void MouseHandler::Update(double deltaTime)
{
	if (!ImGui::GetIO().WantCaptureMouse)
		ApplyMousePan();
}

void MouseHandler::ApplyMousePan()
{
	bool leftClickPressed = GetButton(GLFW_MOUSE_BUTTON_LEFT);

	if (leftClickPressed && !MousePanEnabled)
		Gui->GetMousePosition(MousePanStartX, MousePanStartY);

	MousePanEnabled = leftClickPressed;
}

bool MouseHandler::GetButton(int button)
{
	return glfwGetMouseButton(Gui->GetWindow(), button) == GLFW_PRESS;
}
