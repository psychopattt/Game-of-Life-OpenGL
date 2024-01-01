#include "KeyboardInputs.h"

#include <cmath>

#include "Settings/TransformSettings/TransformSettings.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Inputs/CurrentInputs/CurrentInputs.h"
#include "Settings/Settings.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

using namespace CurrentInputs;
using namespace TransformSettings;

void KeyboardInputs::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
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
				Settings::gui->StepFrame();
			break;
	}
}

void KeyboardInputs::ReleaseCapturedKeys()
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

void KeyboardInputs::Update(double deltaTime)
{
	ReleaseCapturedKeys();
	UpdateSpeedMultiplier();
	UpdatePan(deltaTime);
}

void KeyboardInputs::UpdateSpeedMultiplier()
{
	SpeedMultiplier =
		FastModifierHeld ? FastMultiplier :
		SlowModifierHeld ? SlowMultiplier :
		DefaultMultiplier;
}

void KeyboardInputs::UpdatePan(double deltaTime)
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
