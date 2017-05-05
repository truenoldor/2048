#pragma once
#include "Actor.h"

namespace oxygine
{
	class EventState;
}

using namespace oxygine;



DECLARE_SMART(Modal, spModal);
class Modal: public Actor
{
public:
	Modal();
	virtual ~Modal();
	
	bool generateAction(const std::string &action);
	void skipShowing(bool skip);
	void skipHiding(bool skip);
	void setTransfering(bool enable); // when hiding we want to transfer to another screen, not to parent's
	bool isTransfering() const { return _transfering; }

	std::string loop();
	std::string waitAction();

	std::string showScreenAndWait(spModal screen);
	std::string showDialogAndWait(spModal dialog, bool removeAfterEnd = true);

	virtual void showAndWait();
	virtual void preShowing(){}
	virtual void doShowing();
	virtual void postShowing(){}

	virtual void doLoop(){}

	virtual void hideAndWait();
	virtual void preHiding(){}
	virtual void doHiding();
	virtual void postHiding(){}

	void setFadeOutTime(int fadeOut) { _timeFadeOut = fadeOut; }
	void setFadeInTime(int fadeIn) { _timeFadeIn = fadeIn; }

protected:		
	std::string _action;
	std::string _result;

	timeMS _timeFadeIn;
	timeMS _timeFadeOut;

	bool _skipShowing;
	bool _skipHiding;
	bool _transfering;

	bool	m_fastMode;
};
