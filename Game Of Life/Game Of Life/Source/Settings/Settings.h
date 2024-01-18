#pragma once

namespace Settings
{
	inline class Simulation* Sim = nullptr;
	inline bool ThreadSleep = true;
	inline float TargetFps = 100;
	inline bool EdgeLoop = false;
	inline bool EditMode = false;

	inline class Interface* Gui = nullptr;
	inline bool FullscreenEnabled = false;
	inline bool ShowMetrics = false;
	inline int SelectedFontSize = 1;

	extern class LogString Log;
}
