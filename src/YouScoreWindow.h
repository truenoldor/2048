#pragma once

#include "Popup.h"

namespace oxygine
{
    DECLARE_SMART(YouScoreWindow, spYouScoreWindow);
    class YouScoreWindow : public PopupWindow
    {
    public:
        YouScoreWindow();
        virtual void        init(const std::string & res) OVERRIDE;
        virtual void        onBtnClick(Button * btn)OVERRIDE;

        void                setScores(int scores);

    private:
        spTextField         m_YouScoresNum;
    };
}
