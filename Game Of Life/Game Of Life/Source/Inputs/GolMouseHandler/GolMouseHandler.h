#pragma once

#include "Inputs/MouseHandler/MouseHandler.h"

class GolMouseHandler : public MouseHandler
{
	protected:
		void ApplyMousePan() override;
};
