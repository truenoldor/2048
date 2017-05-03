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
        spTextField makeMeTextField(const std::string & font, int size, TextStyle::HorizontalAlign ha = TextStyle::HALIGN_DEFAULT, TextStyle::VerticalAlign va = TextStyle::VALIGN_DEFAULT);
        void linkTextField(spSprite parent, spTextField tf, Vector2 delta = Vector2(0.f, 0.f));
	}
}