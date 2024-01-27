#include "GolMenu.h"

#include "imgui/imgui.h"

#include "Simulation/GameOfLife.h"
#include "Settings/GameOfLifeSettings.h"
#include "Settings/MainSettings.h"

using namespace ImGui;

void GolMenu::Render()
{
	if (Begin("Settings"))
	{
		if (CollapsingHeader("Game Of Life"))
		{
			if (Checkbox("Edge Loop", &GameOfLifeSettings::EdgeLoop))
				reinterpret_cast<GameOfLife*>(MainSettings::Sim)->ApplySettings();

			Checkbox("Edit Mode", &GameOfLifeSettings::EditMode);
		}
	}

	End();
}
