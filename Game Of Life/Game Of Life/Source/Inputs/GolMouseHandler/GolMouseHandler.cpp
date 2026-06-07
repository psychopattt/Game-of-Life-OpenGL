#include "GolMouseHandler.h"

#include "Settings/GolSettings.h"
#include "Settings/TransformSettings.h"

void GolMouseHandler::ApplyMousePan()
{
	if (GolSettings::EditMode)
	{
		TransformSettings::MousePanEnabled = false;
		return;
	}

	MouseHandler::ApplyMousePan();
}
