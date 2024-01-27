#include "Simulation/GameOfLife.h"
#include "Inputs/GolKeyboardHandler/GolKeyboardHandler.h"
#include "Inputs/GolMouseHandler/GolMouseHandler.h"
#include "Menu/GolMenu.h"
#include "OCSFW.h"

int main()
{
	GameOfLife simulation = GameOfLife(640, 360);
	GolMouseHandler mouseHandler = GolMouseHandler();
	GolKeyboardHandler keyboardHandler = GolKeyboardHandler();

	GolMenu gameOfLifeMenu = GolMenu();
	ImGuiWindow* menus[] = { &gameOfLifeMenu };

	OCSFW(
		&simulation, "Game Of Life", &mouseHandler,
		&keyboardHandler, menus, std::size(menus)
	);

	return EXIT_SUCCESS;
}
