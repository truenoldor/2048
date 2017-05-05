#pragma once
#pragma once

#include "definitions.h"
#include "ui/Button2.h"
#include "ui/Modal.h"
#include "MovieSprite.h"
#include "Board2048.h"

namespace oxygine
{
    DECLARE_SMART(MainMenuScreen, spMainMenuScreen);
    class MainMenuScreen : public Modal
    {
    public:
        //DECLARE_COPYCLONE_NEW(GameScreen);
        MainMenuScreen();
        virtual ~MainMenuScreen();

        virtual void			update(const UpdateState& us)OVERRIDE;
        virtual void			doLoop()OVERRIDE;

        void                    Init();

    private:
        oxygine::Resources      m_Resources;

        spSprite                m_Back;
        spSprite                m_Logo;
        
        spButton2               m_PlayBtn;
        spButton2               m_SettingsBtn;
    };
};
