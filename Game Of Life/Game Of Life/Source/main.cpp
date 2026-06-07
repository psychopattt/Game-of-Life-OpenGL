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

	GolMenu mainMenu = GolMenu();
	GolRulesMenu rulesMenu = GolRulesMenu();
	ImGuiWindow* menus[] = { &mainMenu, &rulesMenu };

	OCSFW(&simulation)
		.WithTitle("Game of Life")
		.WithMouseHandler(&mouseHandler)
		.WithKeyboardHandler(&keyboardHandler)
		.WithMenus(menus, std::size(menus))
		.Run();

	return EXIT_SUCCESS;
}
