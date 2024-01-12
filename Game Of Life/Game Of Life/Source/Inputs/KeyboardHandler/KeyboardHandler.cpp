#include "KeyboardHandler.h"

#include <cmath>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Inputs/CurrentInputs.h"
#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

using namespace CurrentInputs;
using namespace TransformSettings;

void KeyboardHandler::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	HoldingKeys = HoldingKeys || action != GLFW_RELEASE;

	switch (key)
	{
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			UpKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			LeftKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			DownKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			RightKeyHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			FastModifierHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_RIGHT_ALT:
			SlowModifierHeld = action != GLFW_RELEASE;
			break;
		case GLFW_KEY_F:
			if (action != GLFW_RELEASE)
				Settings::Gui->StepFrame();
			break;
	}
}

void KeyboardHandler::ReleaseCapturedKeys()
{
	if (HoldingKeys && ImGui::GetIO().WantCaptureKeyboard)
	{
		UpKeyHeld = false;
		LeftKeyHeld = false;
		DownKeyHeld = false;
		RightKeyHeld = false;
		FastModifierHeld = false;
		SlowModifierHeld = false;
		HoldingKeys = false;
	}
}

void KeyboardHandler::Update(double deltaTime)
{
	ReleaseCapturedKeys();
	UpdateSpeedMultiplier();
	UpdatePan(deltaTime);
}

void KeyboardHandler::UpdateSpeedMultiplier()
{
	SpeedMultiplier =
		FastModifierHeld ? FastMultiplier :
		SlowModifierHeld ? SlowMultiplier :
		DefaultMultiplier;
}

void KeyboardHandler::UpdatePan(double deltaTime)
{
	long long panSpeed = llround(PanMultiplier * SpeedMultiplier * deltaTime);

	if (UpKeyHeld)
		PanY += panSpeed;

	if (LeftKeyHeld)
		PanX -= panSpeed;

	if (DownKeyHeld)
		PanY -= panSpeed;

	if (RightKeyHeld)
		PanX += panSpeed;
}
