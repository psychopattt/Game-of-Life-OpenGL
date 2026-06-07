#include "GolRulesMenu.h"

#include "imgui/imgui.h"

#include "Simulation/GameOfLife.h"
#include "Settings/GameOfLifeSettings.h"
#include "Settings/MainSettings.h"

using namespace ImGui;

GolRulesMenu::GolRulesMenu() : tableFlags(ImGuiTableFlags_RowBg |
	ImGuiTableFlags_BordersInner | ImGuiTableFlags_PadOuterX |
	ImGuiTableFlags_ScrollY) { }

void GolRulesMenu::Render()
{
	if (!GameOfLifeSettings::ShowRulesWindow)
		return;

	PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	SetNextWindowPos(ImVec2(270, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(180, -1), ImGuiCond_FirstUseEver);

	if (Begin("Rules", &GameOfLifeSettings::ShowRulesWindow))
	{
		if (BeginTable("##tableRules", 2, tableFlags))
		{
			TableSetupColumn("Born");
			TableSetupColumn("Survive");
			TableSetupScrollFreeze(0, 1);
			TableHeadersRow();

			for (int row = 0; row < 9; row++)
			{
				TableNextRow();
				TableNextColumn();
				RenderRuleSelectable(GameOfLifeSettings::BirthRules);
				TableNextColumn();
				RenderRuleSelectable(GameOfLifeSettings::SurvivalRules);
			}

			EndTable();
		}
	}

	End();
	PopStyleVar();
}

void GolRulesMenu::RenderRuleSelectable(short& rules)
{
	int row = TableGetRowIndex() - 1;
	int column = TableGetColumnIndex();
	short modifiedRule = 1 << row;

	char label[5];
	snprintf(label, std::size(label), "%d##%d", row, column);

	if (Selectable(label, static_cast<bool>(modifiedRule & rules)))
	{
		rules ^= modifiedRule;
		reinterpret_cast<GameOfLife*>(MainSettings::Sim)->ApplySettings();
	}
}
