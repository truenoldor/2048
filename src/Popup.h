#pragma once

#include "definitions.h"
#include "window.h"
#include "ui/Button2.h"

namespace oxygine
{
    DECLARE_SMART(PopupWindow, spPopupWindow);
    class PopupWindow : public Window
    {
    public:
        PopupWindow();
        virtual ~PopupWindow();

        virtual void            init( const std::string & res );
        virtual void            onBtnClick( Button * );

    protected:
        void                    registerButton(Button * btn);
        void                    setBack(const std::string & res);
    private:
        void                    onClick(Event * ev);        
    private:

        spColorRectSprite       m_BlackScreen;

    protected:
        oxygine::Resources      m_Resources;
        spSprite                m_Back;
    };
}