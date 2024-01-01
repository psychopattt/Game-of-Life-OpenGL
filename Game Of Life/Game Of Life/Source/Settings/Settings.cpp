#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/LogString/LogString.h"

namespace Settings
{
	bool ShowMetrics = false;
	bool ThreadSleep = true;
	float TargetFps = 100;

	CustomInterface* gui = nullptr;
	bool FullscreenEnabled = false;
	int SelectedFontSize = 1;

	LogString log;
}
