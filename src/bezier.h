#pragma once

#include "oxygine-framework.h"
#include <functional>

namespace oxygine
{
	namespace Helper
	{
		oxygine::Vector2 CalcBezierCurve(std::vector<oxygine::Vector2> pts, const double& t);		
	}
}



