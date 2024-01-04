#include "ImGuiDebug.h"

#if DEBUG

#include "imgui/imgui.h"
#include "Shaders/ShaderProvider/ShaderProvider.h"

using namespace ImGui;

void ImGuiDebug::Render()
{
	SetNextWindowSize(ImVec2(200, -1), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(GetMainViewport()->WorkSize.x - 10, 10),
		ImGuiCond_FirstUseEver, ImVec2(1, 0));

	if (Begin("Debug"))
	{
		if (Button("Pack Shaders", ImVec2(-1, 0)))
			ShaderProvider::PackShaders();

		SetItemTooltip("Generate PackedShaders.cpp");
		Checkbox("Show ImGui Demo", &showImGuiDemo);
	}

	End();

	if (showImGuiDemo)
		ShowDemoWindow();
}

#else

void ImGuiDebug::Render() { }

#endif
