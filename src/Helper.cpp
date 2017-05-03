#include "Helper.h"
#include "definitions.h"

namespace oxygine
{
	namespace Helper
	{

        void linkTextField(spSprite parent, spTextField tf, Vector2 delta)
        {
            if (!tf || !parent)
                return;

            Vector2 size(parent->getSize());
            tf->setSize(size);
            tf->setPosition(size.x / 2.f + delta.x, size.y / 2.f + delta.y);
            tf->setAnchor(0.5f, 0.5f);

            parent->addChild(tf);
        }

        spTextField makeMeTextField( const std::string & font, int size, TextStyle::HorizontalAlign ha, TextStyle::VerticalAlign va )
        {
            spTextField text = new TextField;
            TextStyle style;
            style.font = g_GameResources.getResFont(font);
            style.color = Color::White;
            style.vAlign = va;
            style.hAlign = ha;
            style.multiline = true;
            style.fontSize = size;
            text->setStyle(style);
            return text;
        }

		void Rotate(float a, Vector2 & src)
		{
			Vector2 v( src );

			src.x=v.x*cosf(a) - v.y*sinf(a);
			src.y=v.x*sinf(a) + v.y*cosf(a);

		}

		float GetLength( const std::vector< Vector2 > & arr )
		{
			float val = 0.f;

			for ( int i = 1; i < ( int )arr.size(); i++ )
			{
				Vector2 v1 = arr[ i ];
				Vector2 v2 = arr[ i - 1 ];
				val += ( v2 - v1 ).length();
			}
			return val;
		}

		bool VecInterpolation( Vector2 & vCurPos, const Vector2 & vTargetPos, float fCoef, float dt, Vector2 * pvOverFeedBack )
		{
			bool rv = true;
			Vector2 vDir = vTargetPos - vCurPos;
			float fLength = vDir.length();
			if ( fLength < 1.f )
			{
				rv = false;
			}
			else
			{
				float fDelta = fCoef * 333.f * dt / fLength;
				vCurPos += vDir * fDelta;
				if ( vDir.dot( vTargetPos - vCurPos ) < 0.f )
				{
					rv = false;
				}
			}

			if ( !rv )
			{
				if ( pvOverFeedBack )
				{
					*pvOverFeedBack = vCurPos - vTargetPos;
				}
				vCurPos = vTargetPos;
			}
			return rv;
		}


		bool InterpolationFloatSpecial( float & fCur, const float fTarget, float fCoef, float dt, float * pfFeedBack )
		{
			Vector2 vCurPos = Vector2( fCur, 0.f );
			Vector2 vTargetPos = Vector2( fTarget, 0.f );
			bool rv = true;
			Vector2 vDir = vTargetPos - vCurPos;
			float fLength = vDir.length();
			if ( fLength < 0.00001f )
			{
				rv = false;
			}
			else
			{
				float fDelta = fCoef * dt / fLength;
				vCurPos += vDir * fDelta;
				if ( vDir.dot( vTargetPos - vCurPos  ) < 0.f )
				{
					rv = false;
				}
			}

			if ( !rv )
			{
				if ( pfFeedBack )
				{
					*pfFeedBack = vCurPos.x - vTargetPos.x;
				}
				vCurPos = vTargetPos;
			}
			fCur = vCurPos.x;
			return rv;
		}

	}

}

