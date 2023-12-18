#include "ImGuiMain.h"

#include "imgui/imgui.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/Settings.h"

const char* FontSizes[4] = {
    "Small", "Medium", "Large", "Very Large"
};

void ImGuiMain::Render()
{
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(250, 600), ImGuiCond_FirstUseEver);

    ImGui::Begin("Settings");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 15);
    RenderPerformanceSection();
    ImGui::Spacing();
    RenderUiSection();

    ImGui::End();
}

void ImGuiMain::RenderPerformanceSection()
{
    if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Thread Sleep", &Settings::ThreadSleep);
        ImGui::SetItemTooltip(
            "Reduce CPU usage by sleeping as close\n"
            "to 1ms as possible every update\n"
            "Reduces max FPS and FPS stability"
        );

        ImGui::Spacing();
        ImGui::SeparatorText("Target FPS");

        if (ImGui::InputFloat("##textTargetFps", &Settings::TargetFps, 1, 10, "%.2f"))
            UpdateTargetFps();

        if (ImGui::SliderFloat("##sliderTargetFps", &Settings::TargetFps, 0, 500, "%.2f"))
            UpdateTargetFps();
    }
}

void ImGuiMain::RenderUiSection()
{
    if (ImGui::CollapsingHeader("Interface"))
    {
        ImGui::SeparatorText("Font Size");

        bool fontChanged = ImGui::SliderInt(
            "##sliderFontSize", &Settings::SelectedFontSize, 0, 3,
            FontSizes[Settings::SelectedFontSize], ImGuiSliderFlags_NoInput
        );

        if (fontChanged)
        {
            ImGuiIO& io = ImGui::GetIO();
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
