#include "KeyboardInputs.h"

#include "Inputs/CurrentInputs/CurrentInputs.h"
#include "Settings/Settings.h"
#include "GLFW/glfw3.h"

void KeyboardInputs::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	switch (key)
	{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			CurrentInputs::UpKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			CurrentInputs::LeftKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			CurrentInputs::DownKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			CurrentInputs::RightKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			CurrentInputs::FastModifierHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			CurrentInputs::SlowModifierHeld = action != GLFW_RELEASE;
			break;
	}
}

void KeyboardInputs::Update()
{
	UpdatePan();
}

void KeyboardInputs::UpdatePan()
{
	int panSpeed = CurrentInputs::FastModifierHeld ? 30000000 :
		CurrentInputs::SlowModifierHeld ? 300000 : 3000000;

	if (CurrentInputs::UpKeyHeld)
		Settings::CurrentPanY += panSpeed;

	if (CurrentInputs::LeftKeyHeld)
		Settings::CurrentPanX -= panSpeed;

	if (CurrentInputs::DownKeyHeld)
		Settings::CurrentPanY -= panSpeed;

	if (CurrentInputs::RightKeyHeld)
		Settings::CurrentPanX += panSpeed;
}
