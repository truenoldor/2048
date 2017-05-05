#pragma once

#include "definitions.h"
#include "ui/Button2.h"
#include "ui/Modal.h"
#include "MovieSprite.h"
#include "Board2048.h"

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

        static GameScreen *     instance;


        void                    Restart();
        void                    Home();

        void                    YouScoresDlg();

    private:
        void                    InitBoard();
        void                    checkRateUs();

    private:
        oxygine::Resources      m_Resources;

        spSprite                m_Back;
        spButton2               m_MenuBtn;
        spButton2               m_LBBtn;
        spButton2               m_SettingsBtn;
        spSprite                m_ScoresBack;
        spSprite                m_BestBack;

        spBoard2048             m_Board;
	};
};
