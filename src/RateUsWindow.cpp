#include "RateUsWindow.h"
#include "MainScreen.h"
#include "Android_Wrapper.h"
#include "Helper.h"
#include "Localization.h"


namespace oxygine
{
    RateUsWindow::RateUsWindow()
    {

    }

    void RateUsWindow::init(const std::string & res)
    {
        PopupWindow::init(res);

        setBack("back");

        spTextField youScores = Helper::makeMeTextField("a_Alterna-64", 60, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        youScores->setColor(Color(255, 168, 0, 255));
        youScores->setPosition(getSize().x / 2.f - 5.f, 130.f);
        youScores->setAnchor(0.5f, 0.f);
        youScores->setText(g_Localization.GetText("localize_rateus_title"));
        youScores->attachTo(this);

        spCloseWindowButton2 rateNow = new CloseWindowButton2;
        rateNow->CreateTextButton(m_Resources.getResAnim("def_btn"), "CyrillicCompressed-64", 50, "localize_rate_now", 0xfff78dff, (Vector2(-7.f, -15.f)));
        rateNow->setPosition(Vector2(480.f, 300));
        rateNow->setName("rate_now");
        registerButton(rateNow.get());

        spCloseWindowButton2 remindLater = new CloseWindowButton2;
        remindLater->CreateTextButton(m_Resources.getResAnim("def_btn"), "CyrillicCompressed-64", 50, "localize_remind_later", 0xfff78dff, (Vector2(-7.f, -15.f)));
        remindLater->setPosition(Vector2(480.f, 460.f));
        remindLater->setName("remind_later");
        registerButton(remindLater.get());

        spCloseWindowButton2 fuckOff = new CloseWindowButton2;
        fuckOff->CreateTextButton(m_Resources.getResAnim("def_btn2"), "CyrillicCompressed-64", 50, "localize_fuck_off", 0xfff78dff, (Vector2(-7.f, -15.f)));
        fuckOff->setPosition(Vector2(480.f, 620.f));
        fuckOff->setName("fuck_off");
        registerButton(fuckOff.get());

        spCloseWindowButton2 closeBtn = new CloseWindowButton2;
        closeBtn->setResAnim(m_Resources.getResAnim("close_btn"));
        closeBtn->setPosition(Vector2(955.f, 20));
        registerButton(closeBtn.get());

        Vector2 dstPos(getPosition());
        setPosition(Vector2(dstPos.x, getSize().y + GAME_SIZE.y));
        addTween(Actor::TweenPosition(dstPos - Vector2(0.f, 100.f)), 500, 1, false, 0, Tween::ease_inSin);
        addTween(Actor::TweenPosition(dstPos), 200, 1, false, 501, Tween::ease_outSin);
    }

    void RateUsWindow::onBtnClick(Button * btn)
    {
        PopupWindow::onBtnClick(btn);

        if (btn->getName() == "rate_now")
        {
            //rate this
#ifdef __ANDROID__
            Andy_RateNow();
#endif
            Player::instance->m_WasRate = true;
            Player::instance->Save();
            Andy_GoogleAnalytics("rateus", "rate_now");
        }
        else if (btn->getName() == "remind_later")
        {
            Andy_GoogleAnalytics("rateus", "rate_remind");
        }
        else if (btn->getName() == "fuck_off")
        {
            Player::instance->m_WasRate = true;
            Player::instance->Save();
            Andy_GoogleAnalytics("rateus", "rate_nomore");
        }
    }
}
