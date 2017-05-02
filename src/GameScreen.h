#pragma once

#include "definitions.h"
#include "ui/Button2.h"
#include "ui/Modal.h"
#include "MovieSprite.h"

namespace oxygine
{
	DECLARE_SMART(GameScreen, spGameScreen);
	class GameScreen: public Modal
	{
	public:
		//DECLARE_COPYCLONE_NEW(GameScreen);
		GameScreen();
		virtual ~GameScreen();

		virtual void			update(const UpdateState& us)OVERRIDE;
		virtual void			doLoop()OVERRIDE;

        void                    Init();
	};
};
