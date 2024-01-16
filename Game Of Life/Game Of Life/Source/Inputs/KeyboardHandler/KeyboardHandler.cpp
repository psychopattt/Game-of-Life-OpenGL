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
	bool slowModifierHeld = Gui->GetKey(GLFW_KEY_LEFT_ALT) ||
		Gui->GetKey(GLFW_KEY_RIGHT_ALT);
	bool fastModifierHeld = Gui->GetKey(GLFW_KEY_LEFT_SHIFT) ||
		Gui->GetKey(GLFW_KEY_RIGHT_SHIFT);

	SpeedMultiplier =
		slowModifierHeld && !fastModifierHeld ? SlowMultiplier :
		!slowModifierHeld && fastModifierHeld ? FastMultiplier :
		DefaultMultiplier;
}

void KeyboardHandler::ApplyPan(double deltaTime)
{
	long long panSpeed = llround(PanMultiplier * SpeedMultiplier * deltaTime);

	if (Gui->GetKey(GLFW_KEY_W) || Gui->GetKey(GLFW_KEY_UP))
		PanOffsetY += panSpeed;

	if (Gui->GetKey(GLFW_KEY_A) || Gui->GetKey(GLFW_KEY_LEFT))
		PanOffsetX -= panSpeed;

	if (Gui->GetKey(GLFW_KEY_S) || Gui->GetKey(GLFW_KEY_DOWN))
		PanOffsetY -= panSpeed;

	if (Gui->GetKey(GLFW_KEY_D) || Gui->GetKey(GLFW_KEY_RIGHT))
		PanOffsetX += panSpeed;
}
