#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Interface/Interface.h"
#include "Settings/UpdateType.h"

int main()
{
	int uiWidth = 1280;
	int uiHeight = 720;
	int simWidth = 640;
	int simHeight = 360;

	Interface gui(uiWidth, uiHeight, simWidth, simHeight, "Game Of Life");
	SimulationDrawer simDrawer = SimulationDrawer();

	unsigned int* bufferData = new unsigned int[simWidth * simHeight] { };
	DualComputeBuffer dualBuffer(bufferData, simWidth * simHeight * sizeof(*bufferData));
	delete[] bufferData;

	ComputeShader gameInitShader("GameOfLifeInit", simWidth, simHeight);
	gameInitShader.SetInt("width", simWidth);
	gameInitShader.Execute();

	ComputeShader gameShader("GameOfLife", simWidth, simHeight);
	gameShader.SetInt("height", simHeight);
	gameShader.SetInt("width", simWidth);

	while (!gui.ShouldExit())
	{
		UpdateType updateType = gui.Update();

		if (updateType & Simulation)
		{
			gameShader.Execute();
			dualBuffer.Swap();
		}

		if (updateType & Display)
			simDrawer.Draw();
	}

	return EXIT_SUCCESS;
}
