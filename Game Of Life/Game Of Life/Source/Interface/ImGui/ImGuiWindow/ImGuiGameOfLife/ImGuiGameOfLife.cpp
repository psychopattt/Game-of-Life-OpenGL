#include "ImGuiGameOfLife.h"

#include "imgui/imgui.h"

#include "Simulation/GameOfLife/GameOfLife.h"
#include "Settings/Settings.h"

using namespace ImGui;

void ImGuiGameOfLife::Render()
{
	if (Begin("Settings"))
	{
		if (CollapsingHeader("Game Of Life"))
		{
			if (Checkbox("Edge Loop", &Settings::EdgeLoop))
				reinterpret_cast<GameOfLife*>(Settings::Sim)->ApplySettings();
		}
	}

	End();
}
