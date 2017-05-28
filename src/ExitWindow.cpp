#include "ExitWindow.h"
#include "MainScreen.h"
#include "Android_Wrapper.h"
#include "GameScreen.h"
#include "Helper.h"
#include "Localization.h"
#include "MainMenuScreen.h"


namespace oxygine
{
	ExitWindow::ExitWindow()
	{

	}

	void ExitWindow::init(const std::string & res)
	{
		PopupWindow::init(res);

		setBack("back");

		spTextField youScores = Helper::makeMeTextField("a_Alterna-64", 70, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
        youScores->setColor(Color(255, 168, 0, 255));
		youScores->setPosition(getSize().x / 2.f - 5.f, 245.f);
		youScores->setAnchor(0.5f, 0.f);
		youScores->setText(g_Localization.GetText("localize_exit_title"));
		youScores->attachTo(this);

		spCloseWindowButton2 yesBtn = new CloseWindowButton2;
		yesBtn->CreateTextButton(m_Resources.getResAnim("ok_btn"), "CyrillicCompressed-64", 50, "localize_yes", 0xfff78dff, ( Vector2( -7.f, -15.f ) ));
		yesBtn->setPosition(Vector2(290.f, 570));
		yesBtn->setName("yes");
		registerButton(yesBtn.get());

		spCloseWindowButton2 noBtn = new CloseWindowButton2;
		noBtn->CreateTextButton(m_Resources.getResAnim("no_btn"), "CyrillicCompressed-64", 50, "localize_no", 0xfff78dff, (Vector2(-7.f, -15.f)));
		noBtn->setPosition(Vector2(690.f, 570));
		noBtn->setName("no");
		registerButton(noBtn.get());

        spCloseWindowButton2 exitBtn = new CloseWindowButton2;
        exitBtn->CreateTextButton(m_Resources.getResAnim("close_btn"), "", 50, "", 0x000000ff);
        exitBtn->setPosition(Vector2(960.f, 20));
        exitBtn->setName("no");
        registerButton(exitBtn.get());

		Vector2 dstPos(getPosition());
		setPosition(Vector2(dstPos.x, getSize().y - GAME_SIZE.y));
		addTween(Actor::TweenPosition(dstPos + Vector2(0.f, 100.f)), 500, 1, false, 0, Tween::ease_inSin);
		addTween(Actor::TweenPosition(dstPos), 200, 1, false, 501, Tween::ease_outSin);
	}


	void ExitWindow::onBtnClick(Button * btn)
	{
		PopupWindow::onBtnClick(btn);

		if (btn->getName() == "yes")
		{
            Andy_GoogleAnalytics("exit", "exit_yes");

			if (MainMenuScreen::instance)
				MainMenuScreen::instance->Exit();
		}
		else if (btn->getName() == "no")
		{
            Andy_GoogleAnalytics("exit", "exit_no");
		}
	}
}
