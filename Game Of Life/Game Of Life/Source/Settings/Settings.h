#pragma once

namespace Settings
{
	extern bool ShowMetrics;
	extern bool ThreadSleep;
	extern float TargetFps;

	extern class Interface* Gui;
	extern class Simulation* Sim;
	extern bool FullscreenEnabled;
	extern int SelectedFontSize;

	extern class LogString log;
}
