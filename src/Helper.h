#pragma once

#include "oxygine-framework.h"
#include <functional>

namespace oxygine
{
	namespace Helper
	{
		bool VecInterpolation( Vector2 & vCurPos, const Vector2 & vTargetPos, float fCoef, float dt, Vector2 * pvOverFeedBack = NULL );
		bool InterpolationFloatSpecial( float & fCur, const float fTarget, float fCoef, float dt, float * pfFeedBack = NULL );
		void Rotate(float a, Vector2 & src);
		float GetLength( const std::vector< Vector2 > & arr );
	}
}