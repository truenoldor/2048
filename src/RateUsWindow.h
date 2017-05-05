#pragma once

#include "Popup.h"

namespace oxygine
{
    DECLARE_SMART(RateUsWindow, spRateUsWindow);
    class RateUsWindow : public PopupWindow
    {
    public:
        RateUsWindow();
        virtual void        init(const std::string & res) OVERRIDE;
        virtual void        onBtnClick(Button * btn)OVERRIDE;
    };
}
