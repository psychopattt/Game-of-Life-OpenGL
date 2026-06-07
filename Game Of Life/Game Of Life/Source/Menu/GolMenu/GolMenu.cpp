#include "GolMenu.h"

#include "imgui/imgui.h"

#include "Simulation/GameOfLife.h"
#include "Settings/MainSettings.h"
#include "Settings/GolSettings.h"

using namespace ImGui;

void GolMenu::Render()
{
	if (Begin("Settings"))
	{
		if (CollapsingHeader("Game Of Life"))
		{
			if (Checkbox("Edge Loop", &GolSettings::EdgeLoop))
				reinterpret_cast<GameOfLife*>(MainSettings::Sim)->ApplySettings();

			Checkbox("Edit Mode", &GolSettings::EditMode);
			SameLine();
			TextDisabled("[E]");

			Checkbox("Show Rules", &GolSettings::ShowRules);
		}
	}

	End();
}
