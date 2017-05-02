#include "Modal.h"
#include "../blocking.h"
#include "ColorRectSprite.h"
#include "../definitions.h"


class ExitException
{
};


Modal::Modal() : _timeFadeIn(280), _timeFadeOut(280), _skipShowing(false), _skipHiding(false), _transfering(false),
	m_fastMode( true )
{
	setVisible(false);

	spColorRectSprite dimmer = new ColorRectSprite();
	dimmer->setName("dimmer");
	dimmer->setPriority(9999);
	dimmer->setColor(Color(0, 0, 0, 255));
	addChild(dimmer);
}

Modal::~Modal()
{
}

std::string Modal::waitAction()
{
	while (_action.empty())
	{
		if( blocking::yield() == 1 )
		{
			_result = "close";
			return "close";
		}
	}

	_result = _action;
	_action = "";

	return _result;
}

bool Modal::generateAction(const std::string &action)
{
	_action = action;
	return true;
}

void Modal::skipShowing(bool skip)
{
	_skipShowing = skip;
}

void Modal::skipHiding(bool skip)
{
	_skipHiding = skip;
}

void Modal::setTransfering(bool enable)
{
	_transfering = enable;
}

std::string Modal::loop()
{
	showAndWait();
	doLoop();
	hideAndWait();

	return _result;
}

void Modal::showAndWait()
{
	setTouchEnabled(false);

	_action = "";

	if (!_skipShowing)
	{
		preShowing();
		doShowing();
		postShowing();
	}
	_skipShowing = false;

	setTouchEnabled(true);
}

void Modal::hideAndWait()
{
	setTouchEnabled(false);

	if (!_skipHiding)
	{
		preHiding();
		doHiding();
		postHiding();
	}
	_skipHiding = false;

	setTouchEnabled(true);
}

void Modal::doShowing()
{
	spActor dimmer = getChild("dimmer");

	int duration = int(float(_timeFadeIn) * (dimmer->getAlpha() / 255.0f));
	if (m_fastMode)
		duration = 0;

	setVisible(true);
	dimmer->setSize(getSize());

	if (duration > 0)
	{
		spTween t = createTween(Actor::TweenAlpha(0), duration);
		dimmer->addTween(t);

		blocking::waitTween(t);
	}
	else
	{
		setAlpha(255);
		dimmer->setAlpha(0);
	}

	dimmer->setVisible(false);
}

void Modal::doHiding()
{
	spActor dimmer = getChild("dimmer");
	dimmer->setVisible(true);

	int duration = int(float(_timeFadeOut) * (1.0f - dimmer->getAlpha() / 255.0f));
	if (m_fastMode)
		duration = 0;

	if (duration > 0)
	{
		spTween t = createTween(Actor::TweenAlpha(255), duration);
		dimmer->addTween(t);

		blocking::waitTween(t);
	}
	else
	{
		setVisible(false);
		dimmer->setAlpha(255);
	}
}

std::string Modal::showScreenAndWait(spModal screen)
{
	hideAndWait();
	if (getParent())
		detach();

	Stage::instance->addChild(screen);	
	std::string result;
	try
	{
		result = screen->loop();
	}
	catch (ExitException &)
	{
		if (screen->getParent())
			Stage::instance->removeChild(screen);
		throw;
	}
	Stage::instance->removeChild(screen);

	Stage::instance->addChild(this);

	//if (!result.empty()) // result - transports us to another screen, so don't call showAndWait
	//	return result;
	if (!screen->isTransfering())
		showAndWait();

	if (screen->isTransfering())
		screen->setTransfering(false);



	return result;
}

std::string Modal::showDialogAndWait(spModal dialog)
{
	setTouchEnabled(false);
	//	PlaySoundFX( g_SoundResources.get( "snd_splash_appear" ) );

	Stage::instance->addChild(dialog);
	std::string result = dialog->loop();
	Stage::instance->removeChild(dialog);

	setTouchEnabled(true);

	return result;
}