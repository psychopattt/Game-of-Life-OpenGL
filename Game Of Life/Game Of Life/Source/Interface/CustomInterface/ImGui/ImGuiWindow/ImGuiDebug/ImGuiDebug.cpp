#include "ImGuiDebug.h"

#if DEBUG

#include "imgui/imgui.h"
#include "Shaders/ShaderProvider/ShaderProvider.h"

void ImGuiDebug::Render()
{
    float initialWidth = 200;
    float initialPosX = ImGui::GetMainViewport()->WorkSize.x - initialWidth - 50;
    ImGui::SetNextWindowPos(ImVec2(initialPosX, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(initialWidth, 200), ImGuiCond_FirstUseEver);

    ImGui::Begin("Debug");
    float itemWidth = ImGui::GetWindowWidth() - 15;
    ImGui::PushItemWidth(itemWidth);

    if (ImGui::Button("Pack Shaders", ImVec2(itemWidth, 0)))
        ShaderProvider::PackShaders();

    ImGui::SetItemTooltip("Generate PackedShaders.cpp");
    ImGui::End();
}

#else

void ImGuiDebug::Render() { }

#endif
