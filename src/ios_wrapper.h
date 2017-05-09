//
//  ios_wrapper.h
//  game2048_ios
//
//  Created by Мак on 26.09.16.
//  Copyright © 2016 Mac. All rights reserved.
//
#include <string>

#ifndef ios_wrapper_h
#define ios_wrapper_h

//void        GameCenterAuth();
//void        handleGameOver(  int64_t score  );

void SDL_authenticateLocalPlayer ();
void SDL_ShowAchievements (const char * title);
void SDL_SetAchievement (const char *a, int mValue);


void SDL_AddScores( int v );
void SDL_ShowLeaders();

void SDL_SendEmail (const char * a);
void SDL_ShowFacebookPage();
void SDL_ChartBoost();

void SDL_RateApp();

void SDL_SendPushNotification( const char * message );

void SDL_AdMobStart( int h );

std::string iosGetLanguage();

void SDL_StartVideoAD();
void SDL_AdMobHide( bool v );

void SDL_StartFB();
void SDL_RateApp(const char * title, const char * body, const char * yesBtn, const char * cancelBtn );

namespace oxygine
{
    namespace BillingInternal
    {
        void CallEventPurchase( const char * productID, const char * price);
        void CallEventPurchaseFail( const char * productID );
    }
}



extern bool g_AppRateProcess;

extern bool g_ADSoundEnable;


extern bool g_IAPRequestProcess;
extern bool g_GCAuth;
extern bool g_StartProcessVideoAD;
extern bool g_CompleteVideo;

#endif /* ios_wrapper_h */
