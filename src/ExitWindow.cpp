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

		spTextField youScores = Helper::makeMeTextField("bip-2", 70, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
		youScores->setPosition(getSize().x / 2.f, 100.f);
		youScores->setAnchor(0.5f, 0.f);
		youScores->setText(g_Localization.GetText("localize_exit_title"));
		youScores->attachTo(this);

		spCloseWindowButton2 yesBtn = new CloseWindowButton2;
		yesBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_yes", 0x000000ff);
		yesBtn->setPosition(Vector2(200.f, 800));
		yesBtn->setName("yes");
		registerButton(yesBtn.get());

		spCloseWindowButton2 noBtn = new CloseWindowButton2;
		noBtn->CreateTextButton(m_Resources.getResAnim("def_btn"), "bip-2", 50, "localize_no", 0x000000ff);
		noBtn->setPosition(Vector2(600.f, 800));
		noBtn->setName("no");
		registerButton(noBtn.get());

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
			if (MainMenuScreen::instance)
				MainMenuScreen::instance->Exit();
		}
		else if (btn->getName() == "no")
		{
			
		}
	}
}
