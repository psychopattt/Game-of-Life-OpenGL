#pragma once

#include "../ImGuiWindow.h"

class ImGuiMain : public ImGuiWindow
{
    public:
        void Render();

    private:
        void RenderSimulationSection();
        void RenderSimulationPositionSection();
        void RenderSimulationZoomSection();
        void RenderPerformanceSection();
        void RenderInterfaceSection();
};
