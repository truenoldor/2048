#pragma once

#include "Popup.h"

namespace oxygine
{
    DECLARE_SMART(YouScoreWindow, spYouScoreWindow);
    class YouScoreWindow : public PopupWindow
    {
    public:
        YouScoreWindow(bool win, bool withReward);
        virtual ~YouScoreWindow();
        virtual void        init(const std::string & res) OVERRIDE;
        virtual void        onBtnClick(Button * btn)OVERRIDE;

        void                setScores(int scores);

        static YouScoreWindow * instance;

    protected:

        void                doUpdate(const UpdateState & us)OVERRIDE;
    private:
        spTextField         m_YouScoresNum;
        bool                m_IsWin;
        int                 m_TargetScores;
        spTween             m_TweenCollect;
        bool                m_WithReward;
    };
}
