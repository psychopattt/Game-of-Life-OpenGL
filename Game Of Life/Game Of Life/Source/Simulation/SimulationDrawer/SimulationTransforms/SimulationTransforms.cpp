#include "SimulationTransforms.h"

#include <cmath>

#include "Settings/TransformSettings.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"

SimulationTransforms::SimulationTransforms(double* quadVertices, size_t quadVertexCount) :
	quadVertices(quadVertices),	quadVertexCount(quadVertexCount)
{
	auto initialQuadVertices = std::make_unique_for_overwrite<double[]>(quadVertexCount);
	std::copy(quadVertices, quadVertices + quadVertexCount, initialQuadVertices.get());
	this->initialQuadVertices = std::move(initialQuadVertices);
}

bool SimulationTransforms::ApplyTransforms()
{
	bool updatedZoom = ApplyZoom();
	bool updatedPan = ApplyPan();

	if (updatedZoom || updatedPan)
	{
		lastPanX = TransformSettings::PanX;
		lastPanY = TransformSettings::PanY;
		lastZoom = TransformSettings::Zoom;
		TransformSettings::ViewportSizeChanged = false;

		return true;
	}

	return false;
}

bool SimulationTransforms::ApplyZoom()
{
	if (lastZoom == TransformSettings::Zoom && !TransformSettings::ViewportSizeChanged)
		return false;

	double scaledZoom = ScaleZoom(TransformSettings::Zoom);

	// Ensure pixels are square on viewports larger than GL_MAX_VIEWPORT_DIMS
	double scaledPixelX = scaledZoom * TransformSettings::ViewportScaleX;
	double scaledPixelY = scaledZoom * TransformSettings::ViewportScaleY;

	for (size_t i = 0; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] * scaledPixelX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] * scaledPixelY;
	}

	if (TransformSettings::ZoomOnMouse)
		ApplyMouseZoomPan();

	return true;
}

void SimulationTransforms::ApplyMouseZoomPan()
{
	using Settings::Gui, TransformSettings::PanOffsetX, TransformSettings::PanOffsetY;

	double oldZoomMaxPan = ComputeMaxPanAtZoom(lastZoom);
	double newZoomMaxPan = ComputeMaxPanAtZoom(TransformSettings::Zoom);
	double zoomMaxPanDiff = oldZoomMaxPan - newZoomMaxPan;

	double mousePosX, mousePosY;
	Settings::Gui->GetMousePosition(&mousePosX, &mousePosY);

	int viewportWidth, viewportHeight;
	Gui->GetViewportSize(viewportWidth, viewportHeight);

	PanOffsetX = ComputePanOffsetAxis(mousePosX, Gui->GetWidth(), viewportWidth,
		TransformSettings::ViewportScaleX, zoomMaxPanDiff);
	PanOffsetY = -ComputePanOffsetAxis(mousePosY, Gui->GetHeight(), viewportHeight,
		TransformSettings::ViewportScaleY, zoomMaxPanDiff);
}

double SimulationTransforms::ComputeMaxPanAtZoom(unsigned short zoom)
{
	// Calculate pan scale at specified zoom
	double panScale = 1.0 / ScaleZoom(zoom);

	// Multipy max pan without zoom by pan scale
	return TransformSettings::MaxPan * panScale;
}

long long SimulationTransforms::ComputePanOffsetAxis(double screenCoord, double screenSize,
	double viewportSize, double viewportScale, double worldSizeDiff)
{
	// Get screen coord relative to center [-1, 1] from absolute screen coord
	double screenCoordRelativeCenter = 2.0 * screenCoord / screenSize - 1.0;

	// Calculate visible portion of the world size difference
	double visibleWorldSizeRatio = screenSize / viewportSize;
	double visibleWorldSizeDiff = worldSizeDiff * visibleWorldSizeRatio;

	// Map visible world size diff to [-1, 1] by dividing by 2
	// Multiply visible world size diff by relative screen coord to get world coord diff
	double worldCoordDiff = visibleWorldSizeDiff / 2.0 * screenCoordRelativeCenter;

	// Ensure world offset stays valid on viewports larger than GL_MAX_VIEWPORT_DIMS
	worldCoordDiff /= viewportScale;

	return llround(worldCoordDiff);
}

double SimulationTransforms::ScaleZoom(unsigned short zoom)
{
	return pow(1.2, static_cast<double>(zoom) / TransformSettings::FastMultiplier);
}

bool SimulationTransforms::ApplyPan()
{
	using TransformSettings::PanX, TransformSettings::PanY, TransformSettings::PanOffsetX,
		TransformSettings::PanOffsetY, TransformSettings::PanAspectMultiplierX,
		TransformSettings::PanAspectMultiplierY;

	if (lastPanX == PanX && lastPanY == PanY && PanOffsetX == 0 && PanOffsetY == 0)
		return false;

	double panX = ComputePanAxis(lastPanX, PanX, PanAspectMultiplierX, PanOffsetX);
	double panY = ComputePanAxis(lastPanY, PanY, PanAspectMultiplierY, PanOffsetY);

	// Apply new vertex coordinates
	for (size_t i = 2; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] + panX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] + panY;
	}

	return true;
}

double SimulationTransforms::ComputePanAxis(long long& lastPan, long long& currentPan,
	double aspectRatioMultiplier, long long& panOffset)
{
	using TransformSettings::MaxPan, TransformSettings::Zoom, TransformSettings::FastMultiplier;

	// Calculate pan difference
	long long panDiff = currentPan - lastPan;

	// Scale pan difference according to current zoom
	double scale = 1.0 / pow(1.14, static_cast<double>(Zoom) / FastMultiplier);
	long long scaledPanOffset = llround(panDiff * scale);

	// Keep pan speed consistent on all axes
	scaledPanOffset = llround(scaledPanOffset * aspectRatioMultiplier);

	// Ensure the scaled pan difference is at least 1
	if (scaledPanOffset == 0 && panDiff != 0)
		scaledPanOffset = panDiff / abs(panDiff);

	// Add scaled pan difference to last frame's pan
	currentPan = lastPan + scaledPanOffset;

	// Add pan offset from mouse zoom
	currentPan += panOffset;
	panOffset = 0;

	// Loop pan to opposite side if min or max is reached
	if (currentPan < -MaxPan || currentPan > MaxPan)
		currentPan -= currentPan / abs(currentPan) * MaxPan * 2;

	// Convert pan to vertex coordinates
	return static_cast<double>(currentPan) / MaxPan;
}
