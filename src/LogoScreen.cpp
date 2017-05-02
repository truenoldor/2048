#include "LogoScreen.h"



namespace oxygine
{

	void LogoScreen::doLoop()
	{
		while( 1 )
		{
			std::string action = waitAction();
			if (action == "close")
			{
				break;
			}
			if (action == "back")
			{
				break;
			}
			if (action == "play")
			{
				break;
			}
		}
	}

	LogoScreen::LogoScreen():
		m_LoadingTime( 10.f ),
		m_IsLoading( false )
	{
		
	}

	void LogoScreen::update(const UpdateState& us)
	{
		Modal::update( us );
		m_LoadingTime -= ( float )us.dt / 1000.f;
		if ( m_LoadingTime < 0.f && !m_IsLoading )
		{
			generateAction( "play" );
			m_IsLoading = true;
		}		
	}
	void LogoScreen::PreInit()
	{
	}
	

	LogoScreen::~LogoScreen()
	{
		
	}
		

	void LogoScreen::Init()
	{		
		
		spSprite statitcBack = new Sprite();
		statitcBack->setResAnim( g_GameLogoResources.getResAnim("logo") );
		statitcBack->setTouchEnabled( false );
		statitcBack->setBlendMode( blend_disabled );
		addChild( statitcBack );

		m_LoadingTime = 0.3f;
		
// 		spButton2 playBtn = new Button2();
// 		playBtn->setResAnim( g_GameResources.getResAnim("play") );
// 		playBtn->setTouchEnabled( true );
// 		playBtn->setAnchor( 0.5f, 0.f );
// 		playBtn->setPosition( GAME_SIZE.x / 2.f, GAME_SIZE.y - playBtn->getSize().y - 60.f );
// 		addChild( playBtn );
// 
// 		EventCallback cb = CLOSURE(this, &LogoScreen::OnPlay);
// 		playBtn->addEventListener(TouchEvent::CLICK, cb);
// 	
// 		spButton2 gcBtn = new Button2();
// 		gcBtn->setResAnim( g_GameResources.getResAnim("gamecenter") );
// 		gcBtn->setTouchEnabled( true );
// 		gcBtn->setAnchor( 0.f, 0.f );
// 		gcBtn->setPosition( 38.f, GAME_SIZE.y - gcBtn->getSize().y - 60.f );
// 		addChild( gcBtn );
// 
// 		spButton2 settingsBtn = new Button2();
// 		settingsBtn->setResAnim( g_GameResources.getResAnim("settings") );
// 		settingsBtn->setTouchEnabled( true );
// 		settingsBtn->setAnchor( 1.f, 0.f );
// 		settingsBtn->setPosition( GAME_SIZE.x - 38.f, GAME_SIZE.y - settingsBtn->getSize().y - 67.f );
// 		addChild( settingsBtn );
	}

	
	void LogoScreen::OnPlay( Event * pEvent )
	{
		generateAction( "play" );
	}
	
	void LogoScreen::Release()
	{
	}
		
}