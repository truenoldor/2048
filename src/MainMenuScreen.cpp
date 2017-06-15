#include "MainMenuScreen.h"
#include "Helper.h"
#include "MainScreen.h"
#include "ios_wrapper.h"
#include "achievements.h"
#include "Android_Wrapper.h"
#include "Localization.h"
#include "SettingWindow.h"
#include "ExitWindow.h"

#if __ANDROID__
#ifdef HAS_BILLING
#include "billing.h"
#endif
#endif

namespace oxygine
{
	MainMenuScreen * MainMenuScreen::instance = NULL;
    MainMenuScreen::MainMenuScreen()
    {
		MainMenuScreen::instance = this;
    }

    MainMenuScreen::~MainMenuScreen()
    {
		MainMenuScreen::instance = NULL;
        m_Resources.free();
    }

    void MainMenuScreen::Init()
    {        
        m_Resources.loadXML("scripts/res_main.xml", ResourcesLoadOptions().prebuiltFolder("images-high"));

        m_Back = new Sprite;
        m_Back->setResAnim(m_Resources.getResAnim("back"));

        m_PlayBtn = new Button2;
        m_PlayBtn->CreateTextButton(m_Resources.getResAnim("play_btn"), "CyrillicCompressed-64", 50, ""/*"localize_play_btn"*/, 0x000000ff);
        m_PlayBtn->setPosition(Vector2(540.f, 1480.f));
        m_PlayBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
            generateAction("play");
        });

        m_SettingsBtn = new Button2;
        m_SettingsBtn->CreateTextButton(m_Resources.getResAnim("settings_btn"), "CyrillicCompressed-64", 50, ""/*"localize_settings_btn"*/, 0x000000ff);
        m_SettingsBtn->setPosition(Vector2(190.f, 1750.f));
        m_SettingsBtn->setPriority(11);
        m_SettingsBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
            spSettingWindow dlg = new SettingWindow;
            dlg->init("scripts/res_settings.xml");
        });

		m_ExitBtn = new Button2;
		m_ExitBtn->CreateTextButton(m_Resources.getResAnim("exit_btn"), "CyrillicCompressed-64", 50, ""/*"localize_exit_btn"*/, 0x000000ff);
		m_ExitBtn->setPosition(Vector2(900.f, 1750.f));
		m_ExitBtn->setPriority(11);
		m_ExitBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
			spExitWindow dlg = new ExitWindow;
			dlg->init("scripts/res_exit.xml");
		});

//         m_Logo = new Sprite;
//         m_Logo->setResAnim(m_Resources.getResAnim("logo"));
//         m_Logo->setAnchor(0.5f, 0.f);
//         m_Logo->setPosition(Vector2(GAME_SIZE.x / 2.f, 100.f));

        addChild(m_Back);
        addChild(m_PlayBtn);
        addChild(m_SettingsBtn);
		addChild(m_ExitBtn);

        m_PlayBtn->setAlpha(0);
        m_SettingsBtn->setAlpha(0);
        m_ExitBtn->setAlpha(0);

        m_PlayBtn->addTween(Actor::TweenAlpha(255), 1300, 1, false, 0, Tween::ease_outSin);
        m_SettingsBtn->addTween(Actor::TweenAlpha(255), 1300, 1, false, 0, Tween::ease_outSin);
        m_ExitBtn->addTween(Actor::TweenAlpha(255), 1300, 1, false, 0, Tween::ease_outSin);

        PlayMusic1();
    }

	void MainMenuScreen::Exit()
	{
		generateAction( "close" );
	}

    void MainMenuScreen::update(const UpdateState& us)
    {
        Modal::update(us);

        static bool firstTime = true;

        if (firstTime)
        {
            setAlpha(255);
            removeTweens();
            firstTime = false;
        }
    }

    void MainMenuScreen::doLoop()
    {
        while (1)
        {
            std::string action = waitAction();

            if (action == "close")
            {
                g_SessionEnd = true;

                return;
            }
            else if (action == "play")
            {

                return;
            }
        }
    }
}
