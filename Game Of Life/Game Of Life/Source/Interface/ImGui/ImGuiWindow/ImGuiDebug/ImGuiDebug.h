#pragma once

#include "../ImGuiWindow.h"

class ImGuiDebug : public ImGuiWindow
{
	public:
		void Render();

	private:
		bool showImGuiDemo = false;
};
