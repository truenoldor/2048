#include "YouScoreWindow.h"
#include "MainScreen.h"
#include "Android_Wrapper.h"
#include "GameScreen.h"
#include "Helper.h"
#include "Localization.h"


namespace oxygine
{
    YouScoreWindow::YouScoreWindow()
    {

    }

    void YouScoreWindow::init(const std::string & res)
    {
        PopupWindow::init(res);

        setBack("back");

        spTextField youScores = Helper::makeMeTextField("bip-2", 70, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        youScores->setPosition(getSize().x / 2.f, 100.f);
        youScores->setAnchor(0.5f, 0.f);
        youScores->setText(g_Localization.GetText("localize_you_scores_title"));
        youScores->attachTo(this);

        m_YouScoresNum = Helper::makeMeTextField("bip-2", 60, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        m_YouScoresNum->setPosition(getSize().x / 2.f, 300.f);
        m_YouScoresNum->setAnchor(0.5f, 0.f);
        m_YouScoresNum->attachTo(this);


        spCloseWindowButton2 restartBtn = new CloseWindowButton2;
        restartBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_restart", 0x000000ff);
        restartBtn->setPosition(Vector2(200.f, 800));
        restartBtn->setName("restart");
        registerButton(restartBtn.get());

        spCloseWindowButton2 homeBtn = new CloseWindowButton2;
        homeBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_home", 0x000000ff);
        homeBtn->setPosition(Vector2(600.f, 800));
        homeBtn->setName("home");
        registerButton(homeBtn.get());

        Vector2 dstPos(getPosition());
        setPosition(Vector2(dstPos.x, getSize().y - GAME_SIZE.y));
        addTween(Actor::TweenPosition(dstPos + Vector2(0.f, 100.f)), 500, 1, false, 0, Tween::ease_inSin);
        addTween(Actor::TweenPosition(dstPos), 200, 1, false, 501, Tween::ease_outSin);

        PlaySoundFX( "level_complete" );
    }

    void YouScoreWindow::setScores(int scores)
    {
        if (m_YouScoresNum)
        {
            char out[256] = "";
            sprintf(out, "%d", scores);
            m_YouScoresNum->setText(out);
        }
    }

    void YouScoreWindow::onBtnClick(Button * btn)
    {
        PopupWindow::onBtnClick(btn);

        if (btn->getName() == "restart")
        {
            if (GameScreen::instance)
                GameScreen::instance->Restart();

#ifdef __ANDROID__
            Andy_SharedFunc("show_ad", "");
#endif
        }
        else if (btn->getName() == "home")
        {
            if (GameScreen::instance)
                GameScreen::instance->Home();

#ifdef __ANDROID__
            Andy_SharedFunc("show_ad", "");
#endif
        }
    }
}
