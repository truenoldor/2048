#pragma once
#include "Button.h"

using namespace oxygine;

class Button2 : public Button
{
public:
	enum state2
	{
		stateDisabled = 2
	};

	Button2();
	~Button2();

	//void setDisabled(bool disabled);
	//bool isDisabled() const { return _disabled; }

	void					AddSound( const std::string & sound ){ _soundTP = sound; }
	void					ResetBtn(){ m_FirstRes = true; updateButtonState( stateNormal ); }

	void					Press();

	void					setScale(float scaleX, float scaleY);

    void                    setDoubleMask();

protected:
	virtual void			updateButtonState(state s);

	bool _disabled;

	state _savedState;

	std::string				_soundTP;

	bool					m_FirstRes;

	Vector2					m_SaveScale;

private:
	void					_mouseEvent(Event *event);
};

DECLARE_SMART(Button2, spButton2);
