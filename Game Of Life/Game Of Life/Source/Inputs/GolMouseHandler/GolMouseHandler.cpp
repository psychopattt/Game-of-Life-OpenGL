#include "GolMouseHandler.h"

#include "Settings/GolSettings.h"
#include "Settings/TransformSettings.h"
#include "Simulation/SimulationMath/SimulationMath.h"

void GolMouseHandler::ApplyMousePan()
{
	if (GolSettings::EditMode)
	{
		TransformSettings::MousePanEnabled = false;
		UpdateEditModeSettings();
	}
	else
	{
		GolSettings::Editing = false;
		MouseHandler::ApplyMousePan();
	}
}

void GolMouseHandler::UpdateEditModeSettings()
{
	using GolSettings::CurrentPositionX, GolSettings::CurrentPositionY;

	int mousePositionX, mousePositionY;
	SimulationMath::ComputeMousePixelCoords(mousePositionX, mousePositionY);

	GolSettings::LastPositionX = GolSettings::Editing ? CurrentPositionX : mousePositionX;
	GolSettings::LastPositionY = GolSettings::Editing ? CurrentPositionY : mousePositionY;
	CurrentPositionX = mousePositionX;
	CurrentPositionY = mousePositionY;

	GolSettings::Drawing = TransformSettings::DraggingLeftClick;
	GolSettings::Editing = GolSettings::Drawing || TransformSettings::DraggingRightClick;
}
