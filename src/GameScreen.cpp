#include "GameScreen.h"
#include "Helper.h"
#include "MainScreen.h"
#include "ios_wrapper.h"
#include "achievements.h"
#include "Android_Wrapper.h"
#include "Localization.h"
#include "SettingWindow.h"
#include "RateUsWindow.h"
#include "YouScoreWindow.h"

#if __ANDROID__
#ifdef HAS_BILLING
#include "billing.h"
#endif
#endif

namespace oxygine
{
    GameScreen * GameScreen::instance = NULL;
    GameScreen::GameScreen()
    {
        GameScreen::instance = this;
    }

    GameScreen::~GameScreen()
    {
        m_Resources.free();
        GameScreen::instance = NULL;
    }

    void GameScreen::InitBoard()
    {
        m_Board = new Board2048;
        m_Board->setPriority(10);
        m_Board->setPosition(0.f, 500.f);
        m_Board->Create(m_Resources);
        addChild(m_Board);
    }

    void GameScreen::YouScoresDlg()
    {
        spYouScoreWindow dlg = new YouScoreWindow;
        dlg->init("scripts/res_youscore.xml");
        dlg->setScores(m_Board->getScores());
    }

    void GameScreen::Restart()
    {
        if (m_Board)
        {
            m_Board->detach();
            m_Board = 0;
        }

        InitBoard();

        checkRateUs();

    }

    void GameScreen::Home()
    {
        generateAction("menu");
        checkRateUs();
    }

    void GameScreen::checkRateUs()
    {
        if (Player::instance->m_WasRate)
            return;

        Player::instance->m_ShowRate++;
        Player::instance->Save();

        if (Player::instance->m_ShowRate % 5 == 0)
        {
            spRateUsWindow dlg = new RateUsWindow;
            dlg->init("scripts/res_rateus.xml");
        }
    }

    void GameScreen::Init()
    {
        m_Resources.loadXML("scripts/res_game.xml", ResourcesLoadOptions().prebuiltFolder("images-high"));

        m_Back = new Sprite;
        m_Back->setResAnim(m_Resources.getResAnim("back"));

        m_MenuBtn = new Button2;
        m_MenuBtn->CreateTextButton(m_Resources.getResAnim("menu_btn"), "bip-2", 50, "localize_menu_btn", 0x000000ff);
        m_MenuBtn->setPosition(Vector2(600.f, 350.f));
        m_MenuBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
            generateAction("menu");
        });

        m_LBBtn = new Button2;
        m_LBBtn->CreateTextButton(m_Resources.getResAnim("gc_btn"), "bip-2", 50, "localize_lb_btn", 0x000000ff);
        m_LBBtn->setPosition(Vector2(900.f, 350.f));
        m_LBBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
        });

        m_SettingsBtn = new Button2;
        m_SettingsBtn->CreateTextButton(m_Resources.getResAnim("menu_btn"), "bip-2", 50, "localize_settings_btn", 0x000000ff);
        m_SettingsBtn->setPosition(Vector2(200.f, 1750.f));
        m_SettingsBtn->setPriority(11);
        m_SettingsBtn->addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
             spSettingWindow dlg = new SettingWindow;
             dlg->init("scripts/res_settings.xml");
        });

        m_ScoresBack = new Sprite;
        m_ScoresBack->setResAnim(m_Resources.getResAnim("score_back"));
        m_ScoresBack->setPosition(Vector2(500.f, 100.f));

        spTextField textScores = Helper::makeMeTextField("bip-2", 50, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textScores->setColor(Color(0xffffffff));
        textScores->setText(g_Localization.GetText("localize_scores"));
        Helper::linkTextField(m_ScoresBack, textScores, Vector2(0.f, -48.f));

        spTextField textScoresNum = Helper::makeMeTextField("bip-2", 40, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textScoresNum->setColor(Color(0xffffffff));
        textScoresNum->setText("0");
		textScoresNum->setName("num_text");
        Helper::linkTextField(m_ScoresBack, textScoresNum, Vector2(0.f, 28.f));

        m_BestBack = new Sprite;
        m_BestBack->setResAnim(m_Resources.getResAnim("best_back"));
        m_BestBack->setPosition(Vector2(800.f, 100.f));

        spTextField textBest = Helper::makeMeTextField("bip-2", 50, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textBest->setColor(Color(0xffffffff));
        textBest->setText(g_Localization.GetText("localize_best"));
        Helper::linkTextField(m_BestBack, textBest, Vector2(0.f, -48.f));

        spTextField textBestNum = Helper::makeMeTextField("bip-2", 40, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textBestNum->setColor(Color(0xffffffff));
        textBestNum->setText("0");
        textBestNum->setName("num_best");
        Helper::linkTextField(m_BestBack, textBestNum, Vector2(0.f, 28.f));

        InitBoard();

        addChild(m_Back);
        addChild(m_MenuBtn);
        addChild(m_LBBtn);
        addChild(m_SettingsBtn);
        addChild(m_ScoresBack);
        addChild(m_BestBack);
    }

    void GameScreen::update(const UpdateState& us)
    {
        Modal::update(us);
		if (m_ScoresBack)
		{
			spActor t = m_ScoresBack->getChild("num_text");
			if ( TextField * pText = dynamic_cast< TextField* >( t.get() ) )
			{
				char out[256] = "";
				sprintf(out, "%d", m_Board->getScores());
				pText->setText(out);
			}
		}

        if (m_BestBack)
        {
            spActor t = m_BestBack->getChild("num_best");
            if (TextField * pText = dynamic_cast<TextField*>(t.get()))
            {
                char out[256] = "";
                sprintf(out, "%d", Player::instance->m_BestPoints);
                pText->setText(out);
            }
        }
    }

    void GameScreen::doLoop()
    {
        while (1)
        {
            std::string action = waitAction();

            if (action == "close")
            {
                g_SessionEnd = true;

                return;
            }
            else if( action == "menu" )
            {
                return;
            }
        }
    }
}
