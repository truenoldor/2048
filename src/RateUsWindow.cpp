#include "RateUsWindow.h"
#include "MainScreen.h"
#include "Android_Wrapper.h"


namespace oxygine
{
    RateUsWindow::RateUsWindow()
    {

    }

    void RateUsWindow::init(const std::string & res)
    {
        PopupWindow::init(res);

        setBack("back");

        spCloseWindowButton2 rateNow = new CloseWindowButton2;
        rateNow->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_rate_now", 0x000000ff);
        rateNow->setPosition(Vector2(400.f, 300));
        rateNow->setName("rate_now");
        registerButton(rateNow.get());

        spCloseWindowButton2 remindLater = new CloseWindowButton2;
        remindLater->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_remind_later", 0x000000ff);
        remindLater->setPosition(Vector2(400.f, 700.f));
        remindLater->setName("remind_later");
        registerButton(remindLater.get());

        spCloseWindowButton2 fuckOff = new CloseWindowButton2;
        fuckOff->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_fuck_off", 0x000000ff);
        fuckOff->setPosition(Vector2(400.f, 1100.f));
        fuckOff->setName("fuck_off");
        registerButton(fuckOff.get());
    }

    void RateUsWindow::onBtnClick(Button * btn)
    {
        PopupWindow::onBtnClick(btn);

        if (btn->getName() == "rate_now")
        {
            //rate this
            Player::instance->m_WasRate = true;
            Player::instance->Save();
        }
        else if (btn->getName() == "remind_later")
        {

        }
        else if (btn->getName() == "fuck_off")
        {
            Player::instance->m_WasRate = true;
            Player::instance->Save();
        }
    }
}
