#include "MainMenuScreen.h"
#include "Helper.h"
#include "MainScreen.h"
#include "ios_wrapper.h"
#include "achievements.h"
#include "Android_Wrapper.h"
#include "Localization.h"
#include "SettingWindow.h"

#if __ANDROID__
#include "billing.h"
#endif

namespace oxygine
{
    MainMenuScreen::MainMenuScreen()
    {
    }

    MainMenuScreen::~MainMenuScreen()
    {
        m_Resources.free();
    }

    void MainMenuScreen::Init()
    {
        m_Resources.loadXML("scripts/res_main.xml", ResourcesLoadOptions().prebuiltFolder("images-high"));

        m_Back = new Sprite;
        m_Back->setResAnim(m_Resources.getResAnim("back"));

        m_PlayBtn = new Button2;
        m_PlayBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_play_btn", 0x000000ff);
        m_PlayBtn->setPosition(Vector2(800.f, 1750.f));
        m_PlayBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
            generateAction("play");
        });

        m_SettingsBtn = new Button2;
        m_SettingsBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_settings_btn", 0x000000ff);
        m_SettingsBtn->setPosition(Vector2(200.f, 1750.f));
        m_SettingsBtn->setPriority(11);
        m_SettingsBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
            spSettingWindow dlg = new SettingWindow;
            dlg->init("scripts/res_settings.xml");
        });

        m_Logo = new Sprite;
        m_Logo->setResAnim(m_Resources.getResAnim("logo"));
        m_Logo->setAnchor(0.5f, 0.f);
        m_Logo->setPosition(Vector2(GAME_SIZE.x / 2.f, 100.f));

        addChild(m_Back);
        addChild(m_PlayBtn);
        addChild(m_SettingsBtn);
        addChild(m_Logo);
    }

    void MainMenuScreen::update(const UpdateState& us)
    {
        Modal::update(us);
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
                g_SessionEnd = true;

                return;
            }
        }
    }
}
