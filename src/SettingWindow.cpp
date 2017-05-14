#include "SettingWindow.h"
#include "MainScreen.h"
#include "Android_Wrapper.h"


namespace oxygine
{
    SettingWindow::SettingWindow()
    {

    }

    void SettingWindow::init(const std::string & res)
    {
        PopupWindow::init(res);

      //  Andy_SharedFunc("show_ad", ""); //test

        setBack("back");

        spButton2 feedBack = new Button2;
        feedBack->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_feedback", 0x000000ff);
        feedBack->setPosition(Vector2(170.f, 1150.f));
        feedBack->setName("mail");
        registerButton(feedBack.get());

        spButton2 rateUS = new Button2;
        rateUS->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_rateus", 0x000000ff);
        rateUS->setPosition(Vector2(640.f, 1150.f));
        rateUS->setName("rateus");
        registerButton(rateUS.get());

        spCloseWindowButton2 closeBtn = new CloseWindowButton2;
        closeBtn->setResAnim(m_Resources.getResAnim("close_btn"));
        closeBtn->setPosition(Vector2(750.f, 0.f));
        registerButton(closeBtn.get());

        spButton2 music = new Button2;
        music->setResAnim(m_Resources.getResAnim("music"));
        music->setPosition(Vector2(200.f, 250.f));
        music->setName("music");
        registerButton(music.get());
        if (!Player::instance->m_MusicOn)
            music->Press();


        spButton2 sound = new Button2;
        sound->setResAnim(m_Resources.getResAnim("sound"));
        sound->setPosition(Vector2(640.f, 250.f));
        sound->setName("sound");
        registerButton(sound.get());
        if (!Player::instance->m_SoundOn)
            sound->Press();

        Vector2 dstPos( getPosition() );
        setPosition(Vector2(dstPos.x, getSize().y + GAME_SIZE.y));
        addTween(Actor::TweenPosition(dstPos - Vector2( 0.f, 100.f )), 500, 1, false, 0, Tween::ease_inSin);
        addTween(Actor::TweenPosition(dstPos), 200, 1, false, 501, Tween::ease_outSin);
    }

    void SettingWindow::onBtnClick(Button * btn)
    {
        PopupWindow::onBtnClick(btn);

        if ( btn->getName() == "sound" )
        {
            Player::instance->m_SoundOn = !Player::instance->m_SoundOn;

            if (Player::instance->m_SoundOn)
                Andy_GoogleAnalytics("settings", "sound_on");
            else
                Andy_GoogleAnalytics("settings", "sound_off");


            g_MainScreen->SetSoundVolume(Player::instance->m_SoundOn ? 100 : 0);
            ////g_SoundSystem.setVolume( Player::instance->m_SoundOn ? 100.f : 0.f );

            ////g_SoundSystem.setVolume( Player::instance->m_SoundOn ? 100.f : 0.f );
            Player::instance->Save();
        }
        else if (btn->getName() == "music")
        {
            Player::instance->m_MusicOn = !Player::instance->m_MusicOn;

            if (Player::instance->m_MusicOn)
                Andy_GoogleAnalytics("settings", "music_on");
            else
                Andy_GoogleAnalytics("settings", "music_off");

            g_MainScreen->SetMusicVolume(Player::instance->m_MusicOn ? 100 : 0);
            ////g_SoundSystem.setVolume( Player::instance->m_SoundOn ? 100.f : 0.f );

            ////g_SoundSystem.setVolume( Player::instance->m_SoundOn ? 100.f : 0.f );
            Player::instance->Save();
        }
        else if (btn->getName() == "mail")
        {
#ifdef __ANDROID__
        Andy_SendMail();
        Andy_GoogleAnalytics("feedback", "feedback_click");
#endif
        }
        else if (btn->getName() == "rateus")
        {
#ifdef __ANDROID__
            Andy_RateNow();
            //Andy_RateApp("title", "msg", "btn1", "btn2"); // setup later
            //rate this
#endif
            Player::instance->m_WasRate = true;
            Player::instance->Save();
        }
    }
}
