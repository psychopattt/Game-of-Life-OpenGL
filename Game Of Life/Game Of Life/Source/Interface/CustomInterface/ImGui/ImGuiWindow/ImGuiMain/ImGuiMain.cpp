#include "ImGuiMain.h"

#include "imgui/imgui.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/Settings.h"

using namespace ImGui;

const char* FontSizes[4] = {
	"Small", "Medium", "Large", "Very Large"
};

void ImGuiMain::Render()
{
	SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(250, 600), ImGuiCond_FirstUseEver);

	if (Begin("Settings"))
	{
		PushItemWidth(GetWindowWidth() - 15);
		RenderPerformanceSection();
		Spacing();
		RenderUiSection();
	}

	End();
}

void ImGuiMain::RenderPerformanceSection()
{
	if (CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Checkbox("Thread Sleep", &Settings::ThreadSleep);
		SetItemTooltip(
			"Reduce CPU usage by sleeping as close\n"
			"to 1ms as possible every update\n"
			"Reduces max FPS and FPS stability"
		);

		Spacing();
		SeparatorText("Target FPS");

		if (InputFloat("##textTargetFps", &Settings::TargetFps, 1, 10, "%.2f"))
			UpdateTargetFps();

		if (SliderFloat("##sliderTargetFps", &Settings::TargetFps, 0, 500, "%.2f"))
			UpdateTargetFps();
	}
}

void ImGuiMain::RenderUiSection()
{
	if (CollapsingHeader("Interface"))
	{
		if (Checkbox("Fullscreen", &Settings::gui->isFullscreen))
			Settings::gui->ApplyFullscreenState();

		SeparatorText("Font Size");

		bool fontChanged = SliderInt(
			"##sliderFontSize", &Settings::SelectedFontSize, 0, 3,
			FontSizes[Settings::SelectedFontSize], ImGuiSliderFlags_NoInput
		);

		if (fontChanged)
		{
			ImGuiIO& io = GetIO();
			io.FontDefault = io.Fonts->Fonts[Settings::SelectedFontSize];
		}
	}
}

void ImGuiMain::UpdateTargetFps()
{
	if (Settings::TargetFps < 0)
		Settings::TargetFps = 0;

	Settings::ThreadSleep = Settings::TargetFps < 100;
	Settings::gui->SetTargetFps(Settings::TargetFps);
}
