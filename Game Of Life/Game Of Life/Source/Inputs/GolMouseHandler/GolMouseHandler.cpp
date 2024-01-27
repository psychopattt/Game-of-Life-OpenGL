#include "GolMouseHandler.h"

#include "Settings/GameOfLifeSettings.h"
#include "Settings/TransformSettings.h"

void GolMouseHandler::ApplyMousePan()
{
	if (GameOfLifeSettings::EditMode)
	{
		TransformSettings::MousePanEnabled = false;
		return;
	}

	MouseHandler::ApplyMousePan();
}
