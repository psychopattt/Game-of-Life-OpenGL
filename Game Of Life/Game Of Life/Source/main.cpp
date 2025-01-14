#include "Simulation/GameOfLife.h"
#include "Inputs/GolKeyboardHandler/GolKeyboardHandler.h"
#include "Inputs/GolMouseHandler/GolMouseHandler.h"
#include "Menu/GolRulesMenu/GolRulesMenu.h"
#include "Menu/GolMenu/GolMenu.h"
#include "OCSFW.h"

int main()
{
	GameOfLife simulation = GameOfLife(640, 360);
	GolMouseHandler mouseHandler = GolMouseHandler();
	GolKeyboardHandler keyboardHandler = GolKeyboardHandler();

	GolMenu gameOfLifeMenu = GolMenu();
	GolRulesMenu gameOfLifeRulesMenu = GolRulesMenu();
	ImGuiWindow* menus[] = { &gameOfLifeMenu, &gameOfLifeRulesMenu };

	OCSFW(&simulation)
		.WithTitle("Game Of Life")
		.WithMouseHandler(&mouseHandler)
		.WithKeyboardHandler(&keyboardHandler)
		.WithMenus(menus, std::size(menus))
		.Run();

	return EXIT_SUCCESS;
}
