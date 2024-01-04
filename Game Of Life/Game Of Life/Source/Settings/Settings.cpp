#include "Settings/LogString/LogString.h"
#include "Interface/Interface.h"

namespace Settings
{
	bool ShowMetrics = false;
	bool ThreadSleep = true;
	float TargetFps = 100;

	Interface* gui = nullptr;
	bool FullscreenEnabled = false;
	int SelectedFontSize = 1;

	LogString log;
}
