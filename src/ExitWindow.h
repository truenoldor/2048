#pragma once

#include "Popup.h"

namespace oxygine
{
	DECLARE_SMART(ExitWindow, spExitWindow);
	class ExitWindow : public PopupWindow
	{
	public:
		ExitWindow();
		virtual void        init(const std::string & res) OVERRIDE;
		virtual void        onBtnClick(Button * btn)OVERRIDE;		
	};
}
#pragma once
