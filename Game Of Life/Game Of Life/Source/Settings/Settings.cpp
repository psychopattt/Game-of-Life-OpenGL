#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/LogString/LogString.h"

namespace Settings
{
	CustomInterface* gui = nullptr;
	bool ThreadSleep = true;
	float TargetFps = 100;
	int SelectedFontSize = 1;
	LogString log;
}
