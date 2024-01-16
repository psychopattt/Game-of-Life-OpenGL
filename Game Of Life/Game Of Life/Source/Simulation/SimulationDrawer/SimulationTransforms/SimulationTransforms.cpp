#include "SimulationTransforms.h"

#include <cmath>

#include "Interface/Viewport/Viewport.h"
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
	bool updatedTransforms = ApplyPanOffset();
	updatedTransforms = ApplyZoomPan() || updatedTransforms;
	updatedTransforms = ApplyMousePan() || updatedTransforms;
	updatedTransforms = ApplyZoom() || updatedTransforms;
	updatedTransforms = ApplyPan() || updatedTransforms;

	return updatedTransforms;
}

bool SimulationTransforms::ApplyPanOffset()
{
	using TransformSettings::PanOffsetX, TransformSettings::PanOffsetY;

	if (PanOffsetX == 0 && PanOffsetY == 0)
		return false;

	TransformSettings::PanX += ComputePanOffsetAxis(PanOffsetX,
		TransformSettings::PanAspectMultiplierX);
	TransformSettings::PanY += ComputePanOffsetAxis(PanOffsetY,
		TransformSettings::PanAspectMultiplierY);

	return true;
}

long long SimulationTransforms::ComputePanOffsetAxis(
	long long& panOffset, double aspectRatioMultiplier)
{
	using TransformSettings::Zoom, TransformSettings::FastMultiplier;

	if (panOffset == 0)
		return 0;

	// Scale pan offset according to current zoom
	double scale = 1.0 / pow(1.14, static_cast<double>(Zoom) / FastMultiplier);
	long long scaledPanOffset = llround(panOffset * scale);

	// Keep pan speed consistent on all axes
	scaledPanOffset = llround(scaledPanOffset * aspectRatioMultiplier);

	// Ensure the scaled pan offset is at least 1
	if (scaledPanOffset == 0)
		scaledPanOffset = panOffset < 0 ? -1 : 1;

	// Reset pan offset
	panOffset = 0;

	return scaledPanOffset;
}

bool SimulationTransforms::ApplyZoomPan()
{
	using Settings::Gui, TransformSettings::Zoom;

	if (!TransformSettings::ZoomOnMouse || lastZoom == Zoom)
		return false;

	int interfaceWidth, interfaceHeight;
	Gui->GetSize(interfaceWidth, interfaceHeight);

	int viewportWidth, viewportHeight;
	Gui->GetViewport()->GetSize(viewportWidth, viewportHeight);

	double mousePosX, mousePosY;
	Gui->GetMousePosition(mousePosX, mousePosY);

	double zoomMaxPanDiff = GetMaxPanAtZoom(lastZoom) - GetMaxPanAtZoom(Zoom);

	TransformSettings::PanX += ComputeZoomPanAxis(mousePosX, interfaceWidth,
		viewportWidth, TransformSettings::ViewportScaleX, zoomMaxPanDiff);
	TransformSettings::PanY -= ComputeZoomPanAxis(mousePosY, interfaceHeight,
		viewportHeight, TransformSettings::ViewportScaleY, zoomMaxPanDiff);

	return true;
}

long long SimulationTransforms::ComputeZoomPanAxis(double screenCoord, double screenSize,
	double viewportSize, double viewportScale, double worldSizeDiff)
{
	// Get screen coord relative to center [-1, 1] from absolute screen coord
	double screenCoordRelativeCenter = 2.0 * screenCoord / screenSize - 1.0;

	// Calculate visible portion of the world size difference
	double visibleWorldSizeDiff = GetVisibleWorldSize(worldSizeDiff, screenSize, viewportSize);

	// Map visible world size diff to [-1, 1] by dividing by 2
	// Multiply visible world size diff by relative screen coord to get world coord diff
	double worldCoordDiff = visibleWorldSizeDiff / 2.0 * screenCoordRelativeCenter;

	// Ensure world diff stays valid on viewports larger than GL_MAX_VIEWPORT_DIMS
	worldCoordDiff /= viewportScale;

	return llround(worldCoordDiff);
}

bool SimulationTransforms::ApplyMousePan()
{
	using Settings::Gui;

	if (!TransformSettings::MousePanEnabled)
		return false;

	int interfaceWidth, interfaceHeight;
	Gui->GetSize(interfaceWidth, interfaceHeight);

	int viewportWidth, viewportHeight;
	Gui->GetViewport()->GetSize(viewportWidth, viewportHeight);

	double mousePosX, mousePosY;
	Gui->GetMousePosition(mousePosX, mousePosY);

	double zoomMaxPan = GetMaxPanAtZoom(TransformSettings::Zoom);

	TransformSettings::PanX -= ComputeMousePanAxis(TransformSettings::MousePanStartX, mousePosX,
		interfaceWidth, viewportWidth, TransformSettings::ViewportScaleX, zoomMaxPan);
	TransformSettings::PanY += ComputeMousePanAxis(TransformSettings::MousePanStartY, mousePosY,
		interfaceHeight, viewportHeight, TransformSettings::ViewportScaleY, zoomMaxPan);

	return true;
}

long long SimulationTransforms::ComputeMousePanAxis(double& lastScreenCoord, double currentScreenCoord,
	double screenSize, double viewportSize, double viewportScale, double worldSize)
{
	// Calculate screen coord difference and update last coord
	double screenCoordDiff = currentScreenCoord - lastScreenCoord;
	lastScreenCoord = currentScreenCoord;

	// Calculate screen coord diff relative to screen size
	double screenCoordDiffRelative = screenCoordDiff / screenSize;

	// Calculate visible portion of the world size
	double visibleWorldSize = GetVisibleWorldSize(worldSize, screenSize, viewportSize);

	// Multiply visible world size by relative screen coord diff to get world coord diff
	double worldCoordDiff = visibleWorldSize * screenCoordDiffRelative;

	// Ensure world diff stays valid on viewports larger than GL_MAX_VIEWPORT_DIMS
	worldCoordDiff /= viewportScale;

	return llround(worldCoordDiff);
}

bool SimulationTransforms::ApplyZoom()
{
	using TransformSettings::Zoom, TransformSettings::ViewportSizeChanged;

	if (lastZoom == Zoom && !ViewportSizeChanged)
		return false;

	double scaledZoom = ScaleZoom(Zoom);

	// Ensure pixels are square on viewports larger than GL_MAX_VIEWPORT_DIMS
	double scaleX = scaledZoom * TransformSettings::ViewportScaleX;
	double scaleY = scaledZoom * TransformSettings::ViewportScaleY;

	for (size_t i = 0; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] * scaleX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] * scaleY;
	}

	lastZoom = Zoom;
	ViewportSizeChanged = false;

	return true;
}

bool SimulationTransforms::ApplyPan()
{
	using TransformSettings::PanX, TransformSettings::PanY;

	if (lastPanX == PanX && lastPanY == PanY)
		return false;

	double panX = ComputePanAxis(PanX);
	double panY = ComputePanAxis(PanY);

	// Apply new vertex coordinates
	for (size_t i = 2; i < quadVertexCount; i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] + panX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] + panY;
	}

	lastPanX = PanX;
	lastPanY = PanY;

	return true;
}

double SimulationTransforms::ComputePanAxis(long long& pan)
{
	using TransformSettings::MaxPan;

	// Loop pan to opposite side if min or max is reached
	if (pan < -MaxPan || pan > MaxPan)
		pan += (pan < 0 ? 2 : -2) * MaxPan;

	// Convert pan to vertex coordinates
	return static_cast<double>(pan) / MaxPan;
}

double SimulationTransforms::ScaleZoom(double zoom)
{
	return pow(1.2, zoom / TransformSettings::FastMultiplier);
}

double SimulationTransforms::GetMaxPanAtZoom(double zoom)
{
	// Calculate pan scale at specified zoom
	double panScale = 1.0 / ScaleZoom(zoom);

	// Multipy max pan without zoom by pan scale
	return TransformSettings::MaxPan * panScale;
}

double SimulationTransforms::GetVisibleWorldSize(double worldSize,
	double screenSize, double viewportSize)
{
	double visibleWorldSizeRatio = screenSize / viewportSize;
	double visibleWorldSize = worldSize * visibleWorldSizeRatio;

	return visibleWorldSize;
}
