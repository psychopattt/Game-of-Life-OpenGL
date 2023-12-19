#include "ImGuiDebug.h"

#if DEBUG

#include "imgui/imgui.h"
#include "Shaders/ShaderProvider/ShaderProvider.h"

using namespace ImGui;

void ImGuiDebug::Render()
{
	float initialWidth = 200;
	float initialPosX = GetMainViewport()->WorkSize.x - initialWidth - 50;
	SetNextWindowPos(ImVec2(initialPosX, 50), ImGuiCond_FirstUseEver);
	SetNextWindowSize(ImVec2(initialWidth, 200), ImGuiCond_FirstUseEver);

	Begin("Debug");
	float itemWidth = GetWindowWidth() - 15;
	PushItemWidth(itemWidth);

	if (Button("Pack Shaders", ImVec2(itemWidth, 0)))
		ShaderProvider::PackShaders();

	SetItemTooltip("Generate PackedShaders.cpp");
	Checkbox("Show ImGui Demo", &showImGuiDemo);
	End();

	if (showImGuiDemo)
		ShowDemoWindow();
}

#else

void ImGuiDebug::Render() { }

#endif
