#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/LogString/LogString.h"

namespace Settings
{
	CustomInterface* gui = nullptr;
	bool ThreadSleep = true;
	bool ShowMetrics = false;
	bool FullscreenEnabled = false;
	float TargetFps = 100;
	int CurrentZoom = 0;
	int CurrentPanX = 0;
	int CurrentPanY = 0;
	int SelectedFontSize = 1;
	LogString log;
}
