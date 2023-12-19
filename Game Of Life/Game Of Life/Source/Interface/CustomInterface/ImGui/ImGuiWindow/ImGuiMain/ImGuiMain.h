#pragma once

#include "../ImGuiWindow.h"

class ImGuiMain : public ImGuiWindow
{
    public:
        void Render();

    private:
        void RenderPerformanceSection();
        void RenderUiSection();
        void UpdateTargetFps();
};
