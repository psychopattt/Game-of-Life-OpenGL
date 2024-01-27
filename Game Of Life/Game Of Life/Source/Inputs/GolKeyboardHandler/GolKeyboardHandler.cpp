#include "GolKeyboardHandler.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Settings/GameOfLifeSettings.h"

void GolKeyboardHandler::HandleKeyboard(GLFWwindow* window,
	int key, int scanCode, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
		ApplyEditMode(key, action);

	KeyboardHandler::HandleKeyboard(window, key, scanCode, action, mods);
}

void GolKeyboardHandler::ApplyEditMode(int key, int action)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		GameOfLifeSettings::EditMode = !GameOfLifeSettings::EditMode;
}
