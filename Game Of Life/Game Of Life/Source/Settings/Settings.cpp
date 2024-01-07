#include "Settings/LogString/LogString.h"
#include "Simulation/Simulation.h"
#include "Interface/Interface.h"

namespace Settings
{
	bool ShowMetrics = false;
	bool ThreadSleep = true;
	float TargetFps = 100;

	Interface* Gui = nullptr;
	Simulation* Sim = nullptr;
	bool FullscreenEnabled = false;
	int SelectedFontSize = 1;

	LogString log;
}
