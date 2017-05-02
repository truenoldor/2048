#include "ios_wrapper.h"
#include "iap.h"
#include "analitycs_wrap.h"

#if __ANDROID__
#include "billing.h"
#endif

#include "constants.h"

bool g_HasBuyProduct = false;

bool g_GCAuth = false;

bool g_StartProcessVideoAD = false;

bool g_AppRateProcess = false;

bool g_CompleteVideo = false;

bool g_ADSoundEnable = true;

bool g_IAPRequestProcess = false;

void SDL_AdMobStart( int h )
{
}

std::string iosGetLanguage()
{
	return "en";
}

void SDL_AddScores(int)
{
}

void SDL_ShowLeaders()
{
}

void SDL_StartFB()
{
}

void SDL_RateApp(const char * title, const char * body, const char * yesBtn, const char * cancelBtn)
{
}

void SDL_StartVideoAD()
{
}
void SDL_AdMobHide(bool v)
{

}


void EventFlurry (const char *e, const char * par1, const char * par2)
{
}

void SDL_ChartBoost()
{

}

void Purchase_IAP( const char * )
{
	//g_IAPRequestProcess = true;
	//g_HasBuyProduct = true;

#if __ANDROID__
	//g_IAPRequestProcess = true;
	g_HasBuyProduct = false;
	oxygine::billing::purchase( ANDROID_BILLING, "" );
#endif
}

void Restore_IAP()
{
#if __ANDROID__	
	oxygine::billing::consume( ANDROID_BILLING );
#endif
}

void SDL_authenticateLocalPlayer ()
{

}

void SDL_ShowAchievements (const char * title)
{

}

void SDL_SetAchievement (const char *a, int mValue)
{
}

void SDL_SendEmail (const char * a)
{

}
void SDL_ShowFacebookPage()
{

}

void SDL_RateApp()
{
}

void SDL_SendPushNotification( const char * message )
{

}