#pragma once

namespace GolSettings
{
	inline bool EdgeLoop = false;
	inline bool EditMode = false;
	inline bool ShowRules = false;
	inline short BirthRules = 0b1000;
	inline short SurvivalRules = 0b1100;

	inline bool Editing = false;
	inline bool Drawing = false;
	inline int LastPositionX = 0;
	inline int LastPositionY = 0;
	inline int CurrentPositionX = 0;
	inline int CurrentPositionY = 0;
}
