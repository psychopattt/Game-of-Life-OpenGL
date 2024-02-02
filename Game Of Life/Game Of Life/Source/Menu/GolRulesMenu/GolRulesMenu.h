#pragma once

#include "Interface/ImGui/ImGuiWindow/ImGuiWindow.h"

class GolRulesMenu : public ImGuiWindow
{
	public:
		GolRulesMenu();
		void Render() override;

	private:
		const int tableFlags;

		void RenderRuleSelectable(short& rules);
};
