#include "KeyboardHandler.h"

#include <cmath>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Inputs/InputStates/InputStates.h"
#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

using namespace TransformSettings;

KeyboardHandler::KeyboardHandler() : DeviceHandler(GLFW_KEY_LAST) { }

void KeyboardHandler::HandleKeyboard(GLFWwindow* window,
	int key, int scanCode, int action, int mods)
{
	inputStates->Set(key, action != GLFW_RELEASE);
}

void KeyboardHandler::Update(double deltaTime)
{
	ReleaseCapturedKeys();
	ApplySpeedMultiplier();
	ApplyPan(deltaTime);
	ApplyFrameStep();
}

void KeyboardHandler::ReleaseCapturedKeys()
{
	if (ImGui::GetIO().WantCaptureKeyboard)
		inputStates->ResetInputs();
}

void KeyboardHandler::ApplySpeedMultiplier()
{
	bool slowModifierHeld = inputStates->Get(GLFW_KEY_LEFT_ALT) ||
		inputStates->Get(GLFW_KEY_RIGHT_ALT);
	bool fastModifierHeld = inputStates->Get(GLFW_KEY_LEFT_SHIFT) ||
		inputStates->Get(GLFW_KEY_RIGHT_SHIFT);

	SpeedMultiplier =
		slowModifierHeld && !fastModifierHeld ? SlowMultiplier :
		!slowModifierHeld && fastModifierHeld ? FastMultiplier :
		DefaultMultiplier;
}

void KeyboardHandler::ApplyPan(double deltaTime)
{
	long long panSpeed = llround(PanMultiplier * SpeedMultiplier * deltaTime);

	if (inputStates->Get(GLFW_KEY_W) || inputStates->Get(GLFW_KEY_UP))
		PanY += panSpeed;

	if (inputStates->Get(GLFW_KEY_A) || inputStates->Get(GLFW_KEY_LEFT))
		PanX -= panSpeed;

	if (inputStates->Get(GLFW_KEY_S) || inputStates->Get(GLFW_KEY_DOWN))
		PanY -= panSpeed;

	if (inputStates->Get(GLFW_KEY_D) || inputStates->Get(GLFW_KEY_RIGHT))
		PanX += panSpeed;
}

void KeyboardHandler::ApplyFrameStep()
{
	if (inputStates->Pop(GLFW_KEY_F))
		Settings::Gui->StepFrame();
}
