#pragma once
#include <vector>
#include <string>
using namespace std;

void SDL_FlurryEvent (int value, const char * title, const char * e2, const char * e3);
void Andy_Start_App();
void Andy_Stop_App();
void Andy_ShowAchi();
void Andy_ShowLeaderBoard();
void Andy_UnlockAchi( const char * achi, int points );
bool Andy_IsGoogleConnect();
bool Andy_SendMail();
void Andy_SharedFunc( const char * achi, const char * param = NULL );
int Andy_GetPercentObb();
bool Andy_IsGoogle();

void startVideoAd();
void startAdMob();
void hideAdMob();
void Andy_RateApp(const char * title, const char * msg, const char * btn1, const char * btn2);
void Andy_RateNow();


void setScores(int value);