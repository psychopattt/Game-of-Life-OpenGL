#include <memory>

#include "Simulation/GameOfLife/GameOfLife.h"
#include "Interface/Interface.h"
#include "Settings/UpdateType.h"

int main()
{
	std::unique_ptr<Simulation> sim = std::make_unique<GameOfLife>(640, 360);
	Interface gui(1280, 720, "Game Of Life", sim.get());

	sim->Initialize();

	while (!gui.ShouldExit())
	{
		UpdateType updateType = gui.Update();

		if (updateType & SimulationUpdate)
			sim->Execute();

		if (updateType & DisplayUpdate)
			sim->Draw();
	}

	return EXIT_SUCCESS;
}
