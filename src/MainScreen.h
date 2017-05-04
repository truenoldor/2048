#pragma once

#include "definitions.h"
#include "ui/Button2.h"
#include "ui/Modal.h"
#include "MovieSprite.h"
#include "oxygine-sound.h"

#include <map>

#define pushMessage( message, val ) if( g_MainScreen ) g_MainScreen->PushMessage( message, val );
#define pushMessageEx( message, val, holder, options ) if( g_MainScreen ) g_MainScreen->PushMessage( message, val, holder, options );

enum sound_messages
{
	sound_play = 0,
	music_play,
	stop_sound,
	fade_out,
	all_music_fade_out,
	all_music_pause,
	all_music_resume
};

#define PlaySoundFX( name ) pushMessage( "sound_play", name );
#define PlayMusic( name ) pushMessage( "music_play", name )
#define PlaySoundFXHolder( name, ppHolderInst, options ) pushMessageEx( "sound_play", name, ppHolderInst, options )
#define PlaySoundFXEnvironmentHolder( name, ppHolderInst, options ) pushMessageEx( "sound_play_env", name, ppHolderInst, options )
#define PlayMusicFXHolder( name, ppHolderInst, options ) pushMessageEx( "music_play", name, ppHolderInst, options )
#define StopSound( sound ) g_MainScreen->PushMessage( "stop_sound", sound ); sound = NULL;
#define StopSoundEnvironment( sound ) g_MainScreen->PushMessage( "stop_sound_env", sound ); sound = NULL;
#define FadeOutSound( sound ) g_MainScreen->PushMessage( "fade_out", sound ); sound = NULL;
#define FadeOutAllMusic() g_MainScreen->PushMessage( "all_music_fade_out", "1" );
#define PauseAllMusic() g_MainScreen->PushMessage( "all_music_pause", "1" );
#define ResumeAllMusic() g_MainScreen->PushMessage( "all_music_resume", "1" );
#define PauseSoundSystem() g_MainScreen->PushMessage( "pause_ss", "1" );
#define ResumeSoundSystem() g_MainScreen->PushMessage( "resume_ss", "1" );



namespace oxygine
{
	struct SoundMessage
	{
		std::string			m_Message1;
		std::string			m_Message2;
		std::string			m_Message3;

		PlayOptions			m_Options;

		long				m_GlobalSoundIndex;

		SoundMessage():
		m_GlobalSoundIndex( 0 )
		{
		}
	};

	extern ThreadMessages		g_SoundMessages;

	DECLARE_SMART(MainActor, spMainActor);
	class MainActor: public Modal
	{
	public:
		//DECLARE_COPYCLONE_NEW(MainActor);

		//////////////Sound Block

		void					PushMessage( const std::string & message, const std::string & val );
		void					PushMessage( const std::string & message, const std::string & val, long * holder, const PlayOptions & opt );
		void					PushMessage( const std::string & message, long sound );

		void					SkipMessages( bool skipMessages ){ m_SkipMessages = skipMessages; };

		bool					m_SkipMessages;

		void						CastMessageSound( SoundMessage message );

		void					SetVolume( int v );

        void                    SetSoundVolume(int v);
        void                    SetMusicVolume(int v);


		pthread_t				_thread_music;
		static std::map< long, spSoundInstance >		m_SoundsInst;

		static void*			_staticThreadFuncMusic(void* t);
		void					_threadFuncMusic( MainActor* This );

		std::deque< SoundMessage >	m_SoundMessages;

		SoundMessage			GetLastMessage();

		void					AddSoundDebugString( const char * szDebug );

		struct debugSound
		{
			std::string			m_Sound;
			float				m_TimeKill;

			debugSound():
			m_TimeKill( 3.f )
			{
			}
		};
		std::list< debugSound >	m_SoundsDebug;

		spTextField				m_SoundsDebugText;
		void					CreateDebugText();



		//////////////////


		MainActor();

		virtual void			update(const UpdateState& us)OVERRIDE;

		virtual void			doLoop()OVERRIDE;
		void					PreInit();
		void					Init();
		void					Release();
	private:
		bool					m_FirstStart;
	public:

		int						m_SoundVolume;
		int						m_MusicVolume;
		int						m_MusicNum;

	};

	extern bool					g_SessionEnd;
	extern bool					g_ForcedPause;
	extern spMainActor g_MainScreen;
};
