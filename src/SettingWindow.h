#pragma once

#include "Popup.h"

namespace oxygine
{
    DECLARE_SMART(SettingWindow, spSettingWindow);
    class SettingWindow : public PopupWindow
    {
    public:
        SettingWindow();
        virtual void        init(const std::string & res) OVERRIDE;
        virtual void        onBtnClick(Button * btn)OVERRIDE;
    };
}
