#include "Button2.h"
#include "res/ResAnim.h"
#include "../definitions.h"
#include "../MainScreen.h"


Button2::Button2():
	m_FirstRes( true ),
	m_SaveScale( 1.f, 1.f )
{
	_soundTP = "Button"; //test. don`t kill.

    setAnchor(0.5f, 0.5f);

	EventCallback ncb = CLOSURE(this, &Button2::_mouseEvent);
	addEventListener(TouchEvent::TOUCH_DOWN, ncb);
	//_disabled = false;
}
Button2::~Button2()
{
	EventCallback ncb = CLOSURE(this, &Button2::_mouseEvent);
	removeEventListener(TouchEvent::TOUCH_DOWN, ncb);
}

void Button2::setDoubleMask()
{
    spColorRectSprite maskGC = new ColorRectSprite();
    maskGC->setTouchEnabled( true );
    maskGC->setAnchor( 0.5f, 0.5f );
    maskGC->setPosition( getSize().x / 2.f, getSize().y / 2.f );
    maskGC->setSize( getSize() * 1.3f );
    maskGC->setPriority( 1 );
    maskGC->setScale( 1.f, 1.f );
    maskGC->setColor( Color( 0, 0, 0, 0 ) );
    maskGC->attachTo( this );
}

void Button2::Press()
{
	m_FirstRes = false;
	updateButtonState( stateOvered );
}

/*
void Button2::setDisabled(bool disabled)
{
	if (_disabled && !disabled)
	{
		_disabled = disabled;
		setState(_savedState);
	}

	if (!_disabled && disabled)
	{
		if (_resAnim)
		{
			if (_resAnim->getColumns() > stateDisabled)
				setAnimFrame(_resAnim->getFrame(stateDisabled, getRow()));
		}

		_savedState = stateNormal;
	}

	_disabled = disabled;
}*/

void Button2::_mouseEvent(Event *event)
{
	TouchEvent *me = safeCast<TouchEvent*>(event);
	Actor *act = safeCast<Actor*>(event->target.get());

	switch(event->type)
	{
		case TouchEvent::TOUCH_DOWN:
		{
			PlaySoundFX( _soundTP.c_str() );
		}
		break;
	}
}

void Button2::setScale(float scaleX, float scaleY)
{
	m_SaveScale.x = scaleX;
	m_SaveScale.y = scaleY;
	Actor::setScale( scaleX, scaleY );
}


void Button2::updateButtonState(state s)
{
	if (!_resAnim)
		return;

	if (s == Button::stateOvered)
		s = Button::stateNormal;
	else if (s == Button::statePressed)
	{
		m_FirstRes = !m_FirstRes;
		s = Button::stateOvered;

		//g_SoundSystem.PlaySound( "sounds/Button.wav" );


		//PlaySoundFX( g_SoundResources.get("Button")  );
		//PlaySoundFX( "Button" );

		removeTweens();
		addTween(createTween(Actor::TweenScale(Vector2( m_SaveScale.x * 1.1f, m_SaveScale.y * 1.1f )), 150));
		addTween(createTween(Actor::TweenScale(Vector2( m_SaveScale.x , m_SaveScale.y  )), 150, 1, false, 150));
	}


	Sprite::setAnimFrame(_resAnim->getFrame( m_FirstRes || _resAnim->getColumns() == 1 ? 0 : 1, _row));
}
