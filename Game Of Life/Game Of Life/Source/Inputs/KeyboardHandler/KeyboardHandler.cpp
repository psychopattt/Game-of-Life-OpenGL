#include "KeyboardHandler.h"

#include <cmath>

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Settings/TransformSettings.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

using Settings::Gui;
using namespace TransformSettings;

void KeyboardHandler::HandleKeyboard(GLFWwindow* window,
	int key, int scanCode, int action, int mods)
{
	ApplyRestart(key, action);
	ApplyFullscreen(key, action);

	if (!ImGui::GetIO().WantCaptureKeyboard)
		ApplyFrameStep(key, action);
}

void KeyboardHandler::ApplyRestart(int key, int action)
{
	if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		Settings::Sim->Restart();
}

void KeyboardHandler::ApplyFullscreen(int key, int action)
{
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
	{
		Settings::FullscreenEnabled = !Settings::FullscreenEnabled;
		Gui->ApplyFullscreenState();
	}
}

void KeyboardHandler::ApplyFrameStep(int key, int action)
{
	if (key == GLFW_KEY_F && action != GLFW_RELEASE)
		Gui->StepFrame();
}

void KeyboardHandler::Update(double deltaTime)
{
	ApplySpeedMultiplier();

	if (!ImGui::GetIO().WantCaptureKeyboard)
		ApplyPan(deltaTime);
}

void KeyboardHandler::ApplySpeedMultiplier()
{
	bool slowModifierHeld = GetKey(GLFW_KEY_LEFT_ALT) ||
		GetKey(GLFW_KEY_RIGHT_ALT);
	bool fastModifierHeld = GetKey(GLFW_KEY_LEFT_SHIFT) ||
		GetKey(GLFW_KEY_RIGHT_SHIFT);

	SpeedMultiplier =
		slowModifierHeld && !fastModifierHeld ? SlowMultiplier :
		!slowModifierHeld && fastModifierHeld ? FastMultiplier :
		DefaultMultiplier;
}

void KeyboardHandler::ApplyPan(double deltaTime)
{
	long long panSpeed = llround(PanMultiplier * SpeedMultiplier * deltaTime);

	if (GetKey(GLFW_KEY_W) || GetKey(GLFW_KEY_UP))
		PanOffsetY += panSpeed;

	if (GetKey(GLFW_KEY_A) || GetKey(GLFW_KEY_LEFT))
		PanOffsetX -= panSpeed;

	if (GetKey(GLFW_KEY_S) || GetKey(GLFW_KEY_DOWN))
		PanOffsetY -= panSpeed;

	if (GetKey(GLFW_KEY_D) || GetKey(GLFW_KEY_RIGHT))
		PanOffsetX += panSpeed;
}

bool KeyboardHandler::GetKey(int key)
{
	return glfwGetKey(Settings::Gui->GetWindow(), key) == GLFW_PRESS;
}
