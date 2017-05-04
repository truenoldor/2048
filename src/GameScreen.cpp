#include "GameScreen.h"
#include "Helper.h"
#include "MainScreen.h"
#include "ios_wrapper.h"
#include "achievements.h"
#include "Android_Wrapper.h"
#include "Localization.h"

#if __ANDROID__
#include "billing.h"
#endif

namespace oxygine
{
    GameScreen::GameScreen()
    {
    }

    GameScreen::~GameScreen()
    {
        m_Resources.free();
    }

    void GameScreen::Init()
    {
        m_Resources.loadXML("scripts/res_game.xml", ResourcesLoadOptions().prebuiltFolder("images-high"));

        m_Back = new Sprite;
        m_Back->setResAnim(m_Resources.getResAnim("back"));

        m_MenuBtn = new Button2;
        m_MenuBtn->setResAnim(m_Resources.getResAnim("menu_btn"));
        m_MenuBtn->setPosition(Vector2(600.f, 350.f));
        spTextField textMenu = Helper::makeMeTextField("bip-2", 50, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textMenu->setColor(Color(0x000000ff));
        textMenu->setText(g_Localization.GetText("localize_menu_btn"));
        Helper::linkTextField(m_MenuBtn, textMenu, Vector2( 0.f, -6.f ));

        m_LBBtn = new Button2;
        m_LBBtn->setResAnim(m_Resources.getResAnim("gc_btn"));
        m_LBBtn->setPosition(Vector2(900.f, 350.f));
        spTextField textLB = Helper::makeMeTextField("bip-2", 50, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        textLB->setColor(Color(0x000000ff));
        textLB->setText(g_Localization.GetText("localize_lb_btn"));
        Helper::linkTextField(m_LBBtn, textLB, Vector2(0.f, -6.f));

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
        Helper::linkTextField(m_BestBack, textBestNum, Vector2(0.f, 28.f));

        m_Board = new Board2048;
        m_Board->setPriority(10);
        m_Board->setPosition(0.f, 500.f);
        m_Board->Create( m_Resources );


        addChild(m_Back);
        addChild(m_MenuBtn);
        addChild(m_LBBtn);
        addChild(m_ScoresBack);
        addChild(m_BestBack);
        addChild(m_Board);


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
        }
    }
}
