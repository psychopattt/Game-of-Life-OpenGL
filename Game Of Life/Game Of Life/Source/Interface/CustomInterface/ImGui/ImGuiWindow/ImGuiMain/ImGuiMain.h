#pragma once

#include "../ImGuiWindow.h"

class ImGuiMain : public ImGuiWindow
{
    public:
        void Render();

    private:
        void RenderSimulationSection();
        void RenderPerformanceSection();
        void RenderInterfaceSection();
        void UpdateTargetFps();
};
