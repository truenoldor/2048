#include "MainScreen.h"
#include "LogoScreen.h"
#include "GameScreen.h"

#include "tinyxml_p/bind_dynamics.h"


#include "ios_wrapper.h"

#include "blocking.h"



namespace oxygine
{

	spMainActor g_MainScreen;

	MainActor::MainActor():
		m_FirstStart( true ),
		m_SkipMessages( false ),
		m_SoundVolume( 100 ),
		m_MusicVolume( 100 ),
		m_MusicNum( 0 )
	{
	}

    namespace BillingInternal
    {

        void CallEventPurchase( const char * productID, const char * price )
        {
            BillingPurchaseEvent ev(productID, price ? price : "");

            Stage::instance->dispatchEvent(&ev);
        }
        void CallEventPurchaseFail( const char * productID )
        {
            BillingCancelPurchaseEvent ev(productID);

            Stage::instance->dispatchEvent(&ev);
        }
    }

		void* MainActor::_staticThreadFuncMusic(void* t)
		{
			MainActor* This = (MainActor*)t;

			oxygine::SoundSystem::create()->init(16);
			oxygine::SoundPlayer::initialize();

			g_SoundResources.loadXML("sounds/sounds.xml");

			This->_threadFuncMusic( This );

			g_musicPlayer.stop();
			g_sfxPlayer.stop();
			g_sfxEnvirmomentPlayer.stop();
			SoundSystem::instance->stop();
			g_SoundResources.free();
			SoundSystem::instance->release();

			return 0;
		}

		void MainActor::SetVolume( int v )
		{
			m_SoundVolume = v;
			m_MusicVolume = v;
		}

        void MainActor::SetSoundVolume(int v)
        {
            m_SoundVolume = v;
        }

        void MainActor::SetMusicVolume(int v)
        {
            m_MusicVolume = v;
        }

		SoundMessage MainActor::GetLastMessage()
		{
			SoundMessage msg;
			if ( m_SoundMessages.size() )
			{
				msg = m_SoundMessages.back();
				m_SoundMessages.pop_back();
				return msg;
			}

			return msg;
		}

		bool g_ForcedPause = false;

		ThreadMessages		g_SoundMessages;
		bool					g_SessionEnd = false;




		void MainActor::_threadFuncMusic( MainActor* This )
		{
			while( 1 )
			{
				if( g_SessionEnd )
					return;
// 				if ( g_WaitClose || g_SessionEnd )
// 					return;

				sleep( 10 );

				if ( !g_SessionEnd )
				{


					ThreadMessages::peekMessage msg;
					while (g_SoundMessages.peek(msg, true)) {}


					This->m_MusicNum = g_musicPlayer.getSoundsNum();

                    g_sfxPlayer.setVolume( g_ADSoundEnable ? float( This->m_SoundVolume ) / 100.f : 0.f );
                    g_musicPlayer.setVolume( g_ADSoundEnable ? float( This->m_MusicVolume ) / 100.f : 0.f );
                    g_sfxEnvirmomentPlayer.setVolume( g_ADSoundEnable ? float( This->m_MusicVolume ) / 100.f : 0.f );


				}




				oxygine::SoundSystem::get()->update();

// 				if ( g_focusLost || g_IAPRequestProcess )
// 				{
// 					if ( !g_sfxPlayer.IsPaused() )
// 					{
// 						g_sfxPlayer.pause();
// 					}
// 					if ( !g_musicPlayer.IsPaused() )
// 					{
// 						g_musicPlayer.pause();
// 					}
// 					if ( !g_sfxEnvirmomentPlayer.IsPaused() )
// 					{
// 						g_sfxEnvirmomentPlayer.pause();
// 					}
//
// 				}
// 				else
// 				{
// 					if ( g_sfxPlayer.IsPaused() )
// 					{
// 						g_sfxPlayer.resume();
// 					}
//
// 					if ( g_sfxEnvirmomentPlayer.IsPaused() )
// 					{
// 						g_sfxEnvirmomentPlayer.resume();
// 					}
//
//
// 					if ( !g_ForcedPause && g_musicPlayer.IsPaused() )
// 					{
// 						g_musicPlayer.resume();
// 					}
// 				}

				g_sfxPlayer.update();
				g_musicPlayer.update();
				g_sfxEnvirmomentPlayer.update();


			}

		}

		void MainActor::CreateDebugText()
		{
			if ( g_CheatsOn )
			{
				m_SoundsDebugText = new TextField();

				m_SoundsDebugText->attachTo( getStage() );

				m_SoundsDebugText->setPriority( 10000 );

				m_SoundsDebugText->setVisible( true );

				//centered in button

				//initialize text style
				TextStyle style;
				style.font = g_GameResources.getResFont( "bip-2" );
				style.color = Color::White;
				style.vAlign = TextStyle::VALIGN_TOP;
				style.hAlign = TextStyle::HALIGN_LEFT;
				style.multiline = true;
				style.fontSize = 20;

				m_SoundsDebugText->setStyle(style);
				char textS[ 4096 ] = "";
				m_SoundsDebugText->setTouchEnabled( false );

				m_SoundsDebugText->setPosition( 50.f, 5.f );
			}
		}

		void MainActor::AddSoundDebugString( const char * szDebug )
		{
			if ( !g_CheatsOn || !szDebug )
				return;

			debugSound ds;
			ds.m_Sound = szDebug;
			m_SoundsDebug.push_back( ds );
		}

		std::map< long, spSoundInstance > MainActor::m_SoundsInst;

    void MainActor::CastMessageSound( SoundMessage message )
		{
			char debugString[ 4096 ] = "";
            if( message.m_Message1 == "pause_ss" )
            {
                SoundSystem::get()->pause();
                // g_musicPlayer.pause();
            }
            else if( message.m_Message1 == "resume_ss" )
            {
                SoundSystem::get()->resume();
                //g_musicPlayer.resume();
            }
            else if ( message.m_Message1 == "sound_play" )
			{
				spSoundInstance inst = g_sfxPlayer.play( g_SoundResources.get( message.m_Message2, ep_ignore_error ), message.m_Options );
				if( message.m_GlobalSoundIndex )
				{
					MainActor::m_SoundsInst[ message.m_GlobalSoundIndex ] = inst;
				}
			}
			if ( message.m_Message1 == "sound_play_env" )
			{
				spSoundInstance inst = g_sfxEnvirmomentPlayer.play( g_SoundResources.get( message.m_Message2, ep_ignore_error ), message.m_Options );
				if( message.m_GlobalSoundIndex )
				{
					MainActor::m_SoundsInst[ message.m_GlobalSoundIndex ] = inst;
				}
			}
			else if ( message.m_Message1 == "music_play" )
			{
				g_musicPlayer.fadeOut( 500 );
				spSoundInstance inst = g_musicPlayer.play( g_SoundResources.get( message.m_Message2, ep_ignore_error ), message.m_Options );
				if( message.m_GlobalSoundIndex )
				{
					MainActor::m_SoundsInst[ message.m_GlobalSoundIndex ] = inst;
				}
			}
			else if( message.m_Message1 == "stop_sound" )
			{
				if( message.m_GlobalSoundIndex )
				{
					std::map< long, spSoundInstance >::iterator iter = MainActor::m_SoundsInst.find( message.m_GlobalSoundIndex );
					if ( iter != MainActor::m_SoundsInst.end() )
					{
						iter->second->stop();
						MainActor::m_SoundsInst.erase( iter );
					}
				}
			}
			else if( message.m_Message1 == "stop_sound_env" )
			{
				if( message.m_GlobalSoundIndex )
				{
					std::map< long, spSoundInstance >::iterator iter = MainActor::m_SoundsInst.find( message.m_GlobalSoundIndex );
					if ( iter != MainActor::m_SoundsInst.end() )
					{
						iter->second->stop();
						MainActor::m_SoundsInst.erase( iter );
					}
				}
			}
			else if( message.m_Message1 == "fade_out" )
			{
				if( message.m_GlobalSoundIndex )
				{
					std::map< long, spSoundInstance >::iterator iter = MainActor::m_SoundsInst.find( message.m_GlobalSoundIndex );
					if ( iter != MainActor::m_SoundsInst.end() )
					{
						iter->second->fadeOut( 500 );
						MainActor::m_SoundsInst.erase( iter );
					}
				}
			}
			else if( message.m_Message1 == "all_music_fade_out" )
			{
				g_musicPlayer.fadeOut( 500 );
			}
			else if( message.m_Message1 == "all_music_pause" )
			{
				g_musicPlayer.pause();
				g_ForcedPause = true;
			}
			else if( message.m_Message1 == "all_music_resume" )
			{
				g_musicPlayer.resume();
				g_ForcedPause = false;
			}

			bool test = true;

			while (test)
			{
				test = false;
				for (std::map< long, spSoundInstance >::iterator iter = MainActor::m_SoundsInst.begin(), iter_end = MainActor::m_SoundsInst.end(); iter != iter_end; ++iter)
				{
					if (!iter->second || !iter->second->isPlaying())
					{
						MainActor::m_SoundsInst.erase(iter);
						test = true;
						break;
					}
				}
			}
		}

		void MainActor::PushMessage( const std::string & message, long sound )
		{

			if ( m_SkipMessages )
				return;

			if ( !message.length() || !sound )
				return;

			SoundMessage msg;
			msg.m_Message1 = message;
			msg.m_GlobalSoundIndex = sound;



			g_SoundMessages.postCallback([ = ]()
			{
				CastMessageSound( msg );
			}
			);
		}

		long g_LocalSoundIndex = 1;

		void MainActor::PushMessage( const std::string & message, const std::string & val, long * holder, const PlayOptions & opt )
		{

			if ( m_SkipMessages )
				return;

			if ( !message.length() || !val.length() )
				return;

			SoundMessage msg;
			msg.m_Message1 = message;
			msg.m_Message2 = val;
			msg.m_Options = opt;
			msg.m_GlobalSoundIndex = ++g_LocalSoundIndex;
			*holder = msg.m_GlobalSoundIndex;
			//msg.m_ppInstance = holder;

			char debugString[ 4096 ] = "";
			if ( message == "sound_play" )
			{
				sprintf( debugString, "sound_play: %s\n", val.c_str() );
				AddSoundDebugString( debugString );
			}
			else if ( message == "sound_play_env" )
			{
				sprintf( debugString, "sound_play_env: %s\n", val.c_str() );
				AddSoundDebugString( debugString );
			}
			else
			{
				if ( message == "music_play" )
				{
					sprintf( debugString, "music_play: %s\n", val.c_str() );
					AddSoundDebugString( debugString );
				}
			}


			g_SoundMessages.postCallback([ = ]()
			{
				CastMessageSound( msg );
			}
			);
		}

		void MainActor::PushMessage( const std::string & message, const std::string & val )
		{

			if ( m_SkipMessages )
				return;

			if ( !message.length() || !val.length() )
				return;

			SoundMessage msg;
			msg.m_Message1 = message;
			msg.m_Message2 = val;

			char debugString[ 4096 ] = "";
			if ( message == "sound_play" )
			{
				sprintf( debugString, "sound_play: %s\n", val.c_str() );
				AddSoundDebugString( debugString );
			}
			else
			{
				if ( message == "music_play" )
				{
					sprintf( debugString, "music_play: %s\n", val.c_str() );
					AddSoundDebugString( debugString );
				}
			}

			g_SoundMessages.postCallback([ = ]()
			{
				CastMessageSound( msg );
			}
			);

		}

	void MainActor::update(const UpdateState& us)
	{
		Modal::update( us );

		if ( m_FirstStart )
		{
			generateAction( "start_logo_prepare" );
			m_FirstStart = false;
		}

	}

	void MainActor::doLoop()
	{
		while( 1 )
		{
			std::string action = waitAction();

			if ( action == "close" )
			{
				g_SessionEnd = true;

				return;
			}
			else if( action == "start_logo_prepare" )
			{
				g_GameResources.loadXML("scripts/res.xml", ResourcesLoadOptions().prebuiltFolder("images-high"));
				CreateDebugText();
				//blocking::waitTime( 1000 );
				timeMS t1 = getTimeMS();

				g_StartPreGame = true;
				//m_UpPanel->StartScreenScreen();
				generateAction( "start_game" );
				spActor sb = getChild( "static_back" );
				sb->detach();
				sb = NULL;
				timeMS t2 = getTimeMS();

				g_GameLogoResources.free();


				g_blackSpt = new Sprite();
				g_blackSpt->setResAnim( g_GameResources.getResAnim( "black_template" ) );
				g_blackSpt->setScale( 100.f );
				g_blackSpt->setAlpha( 0 );
				//g_blackSpt->addTween( Sprite::TweenAlpha( 128 ), 300 );
				g_blackSpt->setVisible( g_StartPreGame );;
				g_blackSpt->setTouchEnabled( false );
				getStage()->addChild( g_blackSpt );

				g_blackSpt ->setPriority( 14 );
			}
			else if ( action == "start_logo" )
			{
				spLogoScreen scr = new LogoScreen();
				scr->Init();

				std::string message = showDialogAndWait(scr);

				if ( message == "close" )
				{
					scr = NULL;
					_result = "close";
					break;
				}


				if ( message == "play" )
				{
					generateAction( "start_game" );
				}
			}
			else if( action == "start_game" )
			{
				spGameScreen scr = new GameScreen();
				scr->Init();

				std::string message = showDialogAndWait(scr);

				if ( message == "close" )
				{
					scr = NULL;
					_result = "close";
					break;
				}
			}
		}
	}

	void MainActor::PreInit()
	{
	}



	void MainActor::Init()
	{

        getStage()->addEventListener(Stage::ACTIVATE, [](Event*)
                                     {
                                         //SoundSystem::get()->resume();
                                         ResumeSoundSystem();
                                     });

        getStage()->addEventListener(Stage::DEACTIVATE, [](Event*)
                                     {
                                         //SoundSystem::get()->pause();
                                         PauseSoundSystem();
                                         //ArenaGame::instance->killSounds();
                                     });

		pthread_create(&_thread_music, 0, _staticThreadFuncMusic, this);


		log::messageln("Run step 8");

		getStage()->addChild( this );


		spSprite statitcBack = new Sprite();
		statitcBack->setResAnim( g_GameLogoResources.getResAnim("logo") );
		statitcBack->setTouchEnabled( false );
		statitcBack->setName( "static_back" );
		statitcBack->setBlendMode( blend_disabled );
		addChild( statitcBack );

		//generateAction( "start_logo_prepare" );

	}


	void MainActor::Release()
	{
		g_GameResourcesDangerLava.free();
		g_GameResourcesDangerTentacles.free();
	}

}
