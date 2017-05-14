/**
Attention!
This file has Oxygine initialization stuff.
If you just started you don't need to understand it exactly you could check it later.
You could start from example.cpp and example.h it has main functions being called from there
*/
#include "core/oxygine.h"

#include "blocking.h"

#include "Stage.h"
#include "DebugActor.h"


#include "definitions.h"
#include "localization.h"

using namespace oxygine;

#include "MainScreen.h"

#include "ios_wrapper.h"
#include "iap.h"

#include "core/ZipFileSystem.h"

#ifdef __ANDROID__
#include "Android_Wrapper.h"
//#include "../../oxygine/oxygine-admob/src/admob.h"
#endif

#include "achievements.h"

#include "analytics.h"
#if __ANDROID__

#ifdef HAS_BILLING
#include "billing.h"
#endif

#endif

#include "constants.h"

//#include "VungleSDK.h"

bool startAdsVideo = false;

spSprite g_blackSpt;

std::string g_SoundError;
//called each frame
int mainloop()
{
    //example_update();
    //update our stage
    //update all actors. Actor::update would be called also for all children
	//SoundSystem::instance->update();


    Stage::instance->update();

    if (core::beginRendering())
    {
        Color clearColor(32, 32, 32, 255);
        Rect viewport(Point(0, 0), core::getDisplaySize());
        //render all actors. Actor::render would be called also for all children

        if( !g_StartProcessVideoAD )
            Stage::instance->render(clearColor, viewport);

        core::swapDisplayBuffers();
    }

    //update internal components
    //all input events would be passed to Stage::instance.handleEvent
    //if done is true then User requests quit from app.
    bool done = core::update();

    return done ? 1 : 0;
}

long m_Music1 = 0;
long m_Music2 = 0;

void PlayMusic1()
{
	if ( m_Music2 )
	{
		StopSound( m_Music2 );
		m_Music2 = 0;
	}
	if ( !m_Music1 )
		PlayMusicFXHolder( "menu_loop", &m_Music1, PlayOptions().loop() );
}
void PlayMusic2()
{
	if ( m_Music1 )
	{
		StopSound( m_Music1 );
		m_Music1 = 0;
	}
	if( !m_Music2 )
		PlayMusicFXHolder( "game_loop", &m_Music2, PlayOptions().loop() );
}


void ReturnSounds()
{

}



void InitVideoAD()
{




	g_VungleInit = true;
}

void ShowVideoAD()
{


}

bool bigInit = false;
bool isShowBig = false;

void ShowBigAD()
{
	if ( Player::instance->m_IsRemoveAds || !bigInit || !g_BigAdLoadet )
		return;
	isShowBig = true;
}

void InitBigAD()
{
	if ( bigInit || Player::instance->m_IsRemoveAds )
		return;

}

void HideBigAD()
{
	if ( bigInit && isShowBig )
	{
		isShowBig = false;
		bigInit = false;
	}

	InitBigAD();
}


void HideVideoAD()
{
	g_VideoAdsStarted = false;
}

bool g_AdView = false;


void ShowAD()
{
	log::error("Show AD1");
	if(Player::instance->m_IsRemoveAds)
		log::error("ads was deleted");

	if (g_AdView)
		log::error("g_AdView is true");

	if( g_AdView || Player::instance->m_IsRemoveAds )
		return;

#ifdef __APPLE__
	SDL_AdMobHide(false);
#elif __ANDROID__
	log::error("Show AD2");
	startAdMob();
		//admob::show();
#endif



}




void HideAD()
{
	g_AdView = false;
#ifdef __APPLE__
	SDL_AdMobHide(true);
#elif __ANDROID__
	log::error("Show AD2");
	hideAdMob();
	//admob::show();
#endif

}




bool g_GameCenterAuthorize = false;

bool g_IsExistPak = false;


void run()
{
    //ObjectBase::__startTracingLeaks();
	log::messageln("Pre run 1");
#ifdef __ANDROID__
	//admob::init();
	//admob::load();
#endif

	SDL_SetHint("SDL_ANDROID_APK_EXPANSION_MAIN_FILE_VERSION", "12");

	bool allowPack = true;

	if (allowPack)
		g_IsExistPak = oxygine::file::exists("data.pak");

	log::messageln("Pre run 2");

	oxygine::file::ZipFileSystem zp;

	if (g_IsExistPak)
	{
		log::messageln("Pre run 2_1");
		zp.add("data.pak");
		log::messageln("Pre run 2_2");
		oxygine::file::mount(&zp);
		log::messageln("Pre run 2_3");
	}

	log::messageln("Pre run 3");
    //initialize Oxygine's internal stuff
    core::init_desc desc;

#ifndef __APPLE__
    Restore_IAP();
#endif

	log::messageln("Pre run 4");

	//float win_w = 600;
	//float win_h = 800;

	float win_w = 400;
	float win_h = 711;
	//win_w = 1080;
	//win_h = 1920;
	//float win_w = 400;
	//float win_h = 533;

#if OXYGINE_SDL || OXYGINE_EMSCRIPTEN
    //we could setup initial window size on SDL builds
    desc.w = win_w;
    desc.h = win_h;


    //marmalade settings could be changed from emulator's menu
#endif

#ifdef __WIN32__
	desc.force_gles = true;
#endif



	// example_preinit();
     core::init(&desc);

	 log::messageln("Pre run 5");

    //billing::init();
    //billing::requestPurchases();


#if __ANDROID__
	 Andy_Start_App();
#endif

    SDL_authenticateLocalPlayer();

	log::messageln("Pre run 6");


    g_Lang = "";
#ifdef WIN32
	g_Lang = "en";
#endif

    if ( !g_Lang.length() )
    {

#if (defined __APPLE__)
        g_Lang = iosGetLanguage();
#else
        g_Lang = getLanguage();
#endif
    }
    //g_Lang = "en";

    char locF[ 4096 ] = "";

    char pathLoc[ 4096 ] = "";
    sprintf( pathLoc, "localizations-%s.xml", g_Lang.c_str() );

    sprintf( locF, "scripts/%s", pathLoc );

	if (!oxygine::file::exists(locF))
		sprintf(locF, "scripts/localizations-en.xml");


    g_Localization.Init( locF );

    float scaleLocVal = 1.f;

    std::string push_noti = g_Localization.GetText( "push_notification", &scaleLocVal );

#ifdef __APPLE__
    SDL_SendPushNotification( push_noti.c_str() );
#endif
#ifdef __ANDROID__
    Andy_SharedFunc( "notification", push_noti.c_str() );
#endif

    achievements::GetInstance()->deserialize();


    g_MainScreen = new MainActor();

	 g_MainScreen->PreInit();
	////g_SoundSystem.Init();

	InitVideoAD();



	log::messageln("Run step 1");


	//g_sfxPlayer.play(g_SoundResources.get("buy_universal"), PlayOptions().fade(1500));
	//g_musicPlayer.play(g_SoundResources.get("preview"), PlayOptions().loop().fade(1500));




// 	SoundPlayer::instance.setResources(&g_SoundResources);
	g_MainScreen->SetVolume( 100 );


	Player::instance = new Player();
	Player::instance->Load();




	PlayMusic2();


	log::messageln("Run step 2");

	g_GameLogoResources.loadXML( "scripts/res_logo.xml", ResourcesLoadOptions().prebuiltFolder( "images-high" ) );

	log::messageln("Run step 3");






	log::messageln("Run step 4");
    //create Stage. Stage is a root node
    Stage::instance = new Stage(false);

    Point size = core::getDisplaySize();
    if( size.x > size.y )
    {
       // std::swap( size.x, size.y );
    }


    if ( !Player::instance->m_IsRemoveAds )
    {
        int h = int( float( size.y ) / 1024.f * 50.f );
        SDL_AdMobStart( h );
    }

	log::messageln("Run step 5");
#ifdef __WIN32__
	size.x = win_w;
	size.y = win_h;
#endif
	DEVICE_SIZE = size;
    getStage()->setSize( Vector2( 2048.f, 2048.f )/*GAME_SIZE*/ );
	getStage()->setAnchor( 0.f, 0.f );
	Vector2 scale = Vector2(  size.x / GAME_SIZE.x, size.y / GAME_SIZE.y );
	#define max(a,b)            (((a) > (b)) ? (a) : (b))
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
	float minScale = min( scale.x, scale.y );
// 	scale.x = minScale;
// 	scale.y = minScale;
	getStage()->setScale( scale );

	log::messageln("Run step 6");

	SDL_Window* pWindow = core::getWindow();
	SDL_SetWindowSize( pWindow, size.x, size.y );
    //DebugActor is a helper actor node. It shows FPS, memory usage and other useful stuff
    DebugActor::show();


    //initialize this example stuff. see example.cpp
	blocking::setYieldCallback(mainloop);
	MovieSprite::init(true);
	log::messageln("Run step 7");
    g_MainScreen->Init();
	g_MainScreen->loop();
	Player::instance->Save();
	g_MainScreen->detach();



	g_GameResources.free();

#ifdef __ANDROID__
	//admob::free();
#endif


	if (g_IsExistPak)
	{
		oxygine::file::unmount(&zp);
	}

#ifdef EMSCRIPTEN
    /*
    if you build for Emscripten mainloop would be called automatically outside.
    see emscripten_set_main_loop below
    */
    return;
#endif


    //here is main game loop
//     while (1)
//     {
//         int done = mainloop();
//         if (done)
//             break;
//     }
    //user wants to leave application...

    //lets dump all created objects into log
    //all created and not freed resources would be displayed


    //lets cleanup everything right now and call ObjectBase::dumpObjects() again
    //we need to free all allocated resources and delete all created actors
    //all actors/sprites are smart pointer objects and actually you don't need it remove them by hands
    //but now we want delete it by hands

    //check example.cpp
	g_MainScreen->Release();
	g_MainScreen = NULL;

	MovieSprite::free();


    //renderer.cleanup();

    /**releases all internal components and Stage*/
    core::release();



    //dump list should be empty now
    //we deleted everything and could be sure that there aren't any memory leaks
    //ObjectBase::dumpCreatedObjects();

    //ObjectBase::__stopTracingLeaks();
    //end
}



#ifdef OXYGINE_SDL

#include "SDL_main.h"
extern "C"
{
    int main(int argc, char* argv[])
    {
        run();
        return 0;
    }
};
#endif

#ifdef EMSCRIPTEN
#include <emscripten.h>

void one() { mainloop(); }

int main(int argc, char* argv[])
{
    run();
    emscripten_set_main_loop(one, 0, 0);
    return 0;
}
#endif
