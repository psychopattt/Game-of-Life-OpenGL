#include "ImGuiMain.h"

#include "imgui/imgui.h"
#include "Settings/TransformSettings/TransformSettings.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/Settings.h"

using namespace ImGui;

static const char* FontSizes[] = {
	"Small", "Medium", "Large", "Very Large"
};

void ImGuiMain::Render()
{
	SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(250, -1), ImGuiCond_FirstUseEver);

	if (Begin("Settings"))
	{
		PushItemWidth(-1);
		RenderSimulationSection();
		Spacing();
		RenderPerformanceSection();
		Spacing();
		RenderInterfaceSection();
		PopItemWidth();
	}

	End();
}

void ImGuiMain::RenderSimulationSection()
{
	if (CollapsingHeader("Simulation", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SeparatorText("Position");
		long long position[] = { TransformSettings::PanX, TransformSettings::PanY };
		
		if (SliderScalarN("##sliderPosition", ImGuiDataType_S64, position,
			static_cast<int>(std::size(position)), &TransformSettings::MinUiPan,
			&TransformSettings::MaxUiPan))
		{
			TransformSettings::PanX = position[0];
			TransformSettings::PanY = position[1];
		}

		SeparatorText("Zoom");
		if (RadioButton("Mouse", TransformSettings::ZoomOnMouse))
			TransformSettings::ZoomOnMouse = true;
		
		SetItemTooltip("Zoom on the mouse cursor");
		SameLine();
		
		if (RadioButton("Center", !TransformSettings::ZoomOnMouse))
			TransformSettings::ZoomOnMouse = false;
		
		SetItemTooltip("Zoom on the center of the screen");

		if (SliderScalar("##sliderZoom", ImGuiDataType_U16,
			&TransformSettings::Zoom, &TransformSettings::MinUiZoom,
			&TransformSettings::MaxUiZoom))
		{
			TransformSettings::ZoomOnMouse = false;
		}
	}
}

void ImGuiMain::RenderPerformanceSection()
{
	if (CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Checkbox("Show Metrics", &Settings::ShowMetrics);
		Checkbox("Thread Sleep", &Settings::ThreadSleep);
		SetItemTooltip(
			"Reduce CPU usage by sleeping as close\n"
			"to 1ms as possible every update\n"
			"Reduces max FPS and FPS stability"
		);

		Spacing();
		SeparatorText("Target FPS");

		if (InputFloat("##textTargetFps", &Settings::TargetFps, 1, 10, "%.2f"))
			Settings::gui->SetTargetFps(Settings::TargetFps);

		if (SliderFloat("##sliderTargetFps", &Settings::TargetFps, 0, 500, "%.2f"))
			Settings::gui->SetTargetFps(Settings::TargetFps);

		if (Button("Step Frame", ImVec2(-1, 0)))
			Settings::gui->StepFrame();
	}
}

void ImGuiMain::RenderInterfaceSection()
{
	if (CollapsingHeader("Interface"))
	{
		if (Checkbox("Fullscreen", &Settings::FullscreenEnabled))
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
