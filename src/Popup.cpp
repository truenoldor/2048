#include "Popup.h"
#include "ui/Button2.h"
#include "MainScreen.h"

namespace oxygine
{
    PopupWindow::PopupWindow()
    {

    }

    PopupWindow::~PopupWindow()
    {
        m_Resources.unload();
    }

    void PopupWindow::setBack(const std::string & res)
    {
        m_Back = new Sprite;
        m_Back->setResAnim(m_Resources.getResAnim(res));

        m_Back->attachTo(this);

        m_Back->setAnchor(0.5f, 0.5f);

        setSize(m_Back->getSize());
        setAnchor(0.5f, 0.5f);

        setPosition(GAME_SIZE.x / 2.f, GAME_SIZE.y / 2.f);

        m_Back->setPosition(getSize().x / 2.f, getSize().y / 2.f);
        m_BlackScreen->setPosition(getSize().x / 2.f, -getSize().y / 2.f);
    }

    void PopupWindow::onBtnClick( Button * )
    {

    }

    void PopupWindow::init(const std::string & res)
    {
        m_Resources.loadXML(res, ResourcesLoadOptions().prebuiltFolder("images-high"));

        m_BlackScreen = new ColorRectSprite;
        m_BlackScreen->setSize(Vector2( GAME_SIZE.x, GAME_SIZE.y * 4.f));
        m_BlackScreen->setColor( Color( 0x000000ff ) );
        m_BlackScreen->setAlpha(128);
        m_BlackScreen->setAnchor(0.5f, 0.5f);
        m_BlackScreen->attachTo(this);

        setAlpha(0);
        addTween(Actor::TweenAlpha(255), 400, 1, false, 0, Tween::ease_inOutQuad);

        setPriority(10000);
        attachTo(Stage::instance);

        PlaySoundFX( "splash_apear" );
    }

    void PopupWindow::onClick(Event * ev)
    {
        CloseWindowButton2 * btn = dynamic_cast< CloseWindowButton2 * >(ev->currentTarget.get());
        if (btn)
        {
            spTweenQueue tq2 = new TweenQueue;
            tq2->add(Actor::TweenAlpha(0), 400, 1, false, 0, Tween::ease_inOutQuad);
            addTween(tq2);
            tq2->addDoneCallback([this](Event * e)->void
            {
                detach();
            }
            );
        }

        if (Button * btn = dynamic_cast< Button * >(ev->currentTarget.get()))
        {
            onBtnClick( btn );
        }
    }

    void PopupWindow::registerButton(Button * btn)
    {
        if (!btn)
            return;

        EventCallback cb = CLOSURE(this, &PopupWindow::onClick);
        btn->addEventListener(TouchEvent::TOUCH_DOWN, cb);
        btn->attachTo(this);
    }
}