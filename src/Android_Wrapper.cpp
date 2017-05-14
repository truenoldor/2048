#include <jni.h>
#include <android/log.h>
#include <assert.h>
#include "core/oxygine.h"
#include "core/Object.h"
#include "core/ThreadDispatcher.h"
#include "core/oxygine.h"
#include "core/android/jniHelper.h"
#include "core/android/jniUtils.h"

#include "Android_Wrapper.h"

using namespace oxygine;

void SDL_FlurryEvent (int value, const char * title, const char * e2, const char * e3)
{
	log::message("Start flurry ev");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "FlurryEvent", "(Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)Z");
	if (mid) {
		log::message("find mid");
		jstring jtitle = (jstring)(mEnv->NewStringUTF( title));
		jstring jtitle1 = (jstring)(mEnv->NewStringUTF( e2));
		jstring jtitle2 = (jstring)(mEnv->NewStringUTF( e3));
		mEnv->CallStaticBooleanMethod(mActivityClass, mid, jtitle,value,jtitle1,jtitle2);
		mEnv->DeleteLocalRef(jtitle);
		mEnv->DeleteLocalRef(jtitle1);
		mEnv->DeleteLocalRef(jtitle2);
	}

}

JNIEnv * sg_JniInv = NULL;
jmethodID obbMethod;
jclass mActivityClassPercObb;

int Andy_GetPercentObb()
{
	if ( obbMethod )
	{
		return sg_JniInv->CallStaticIntMethod(mActivityClassPercObb, obbMethod);
	}

	JNIEnv *mEnv = sg_JniInv ? sg_JniInv : jniGetEnv();
	sg_JniInv = mEnv;
	mActivityClassPercObb = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	obbMethod= mEnv->GetStaticMethodID(mActivityClassPercObb, "getPercentObb", "()I");
	if (obbMethod) {
		int val = mEnv->CallStaticIntMethod(mActivityClassPercObb, obbMethod);
		return val;
	}

	return 0;
}

void Andy_ShowAchi()
{
	log::message("Andy_ShowAchi:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "showAchievements", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticBooleanMethod(mActivityClass, mid);
	}
}


void Andy_ShowLeaderBoard()
{
	log::message("Andy_ShowLeaderBoard:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "showLeaderBoard", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticBooleanMethod(mActivityClass, mid);
	}
}

void startAdMob()
{
	log::message("startVideoAd:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "showAd", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid);
	}
}

void Andy_RateNow()
{
    log::message("Andy_RateNow:");
    jmethodID mid;
    JNIEnv *mEnv = jniGetEnv();
    jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

    if (mActivityClass)
    {
        log::message("find mActivityClass");
    }

    mid = mEnv->GetStaticMethodID(mActivityClass, "rateNow", "()V");
    if (mid) {
        log::message("find mid");
        mEnv->CallStaticVoidMethod(mActivityClass, mid);
    }
}

void Andy_RateApp( const char * title, const char * msg, const char * btn1, const char * btn2 )
{
	log::message("rateApp:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "rateApp", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (mid) {
		log::message("find mid");
		jstring jtitle = (jstring)(mEnv->NewStringUTF(title));
		jstring jmsg = (jstring)(mEnv->NewStringUTF(msg));
		jstring jbtn1 = (jstring)(mEnv->NewStringUTF(btn1));
		jstring jbtn2 = (jstring)(mEnv->NewStringUTF(btn2));
		mEnv->CallStaticVoidMethod(mActivityClass, mid, jtitle, jmsg, jbtn1, jbtn2);
		mEnv->DeleteLocalRef(jtitle);
		mEnv->DeleteLocalRef(jmsg);
		mEnv->DeleteLocalRef(jbtn1);
		mEnv->DeleteLocalRef(jbtn2);
	}
}

void hideAdMob()
{
	log::message("startVideoAd:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "hideAd", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid);
	}
}

void startVideoAd()
{
	log::message("startVideoAd:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "startVideoAd", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid);
	}
}



void setScores(int value)
{
	log::message("setScores");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if (mActivityClass)
	{
		log::message("find mActivityClass");
	}

	mid = mEnv->GetStaticMethodID(mActivityClass, "setScores", "(I)V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid, value);
	}
}

bool Andy_IsGoogleConnect()
{
	log::message("Andy_ShowAchi:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "isAchiAllow", "()Z");
	if (mid) {
		log::message("find mid");
		bool isGC = mEnv->CallStaticBooleanMethod(mActivityClass, mid);
		return isGC;
	}

	return false;
}

bool Andy_SendMail()
{
	log::message("Andy_SendMail:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "sendEmail", "()Z");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticBooleanMethod(mActivityClass, mid);
	}

	return false;
}

void Andy_UnlockAchi( const char * achi, int points )
{
	log::message("unlock achi: %s", achi);
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "setAchievement", "(Ljava/lang/String;I)Z");
	if (mid) {
		log::message("find mid");
		jstring jtitle = (jstring)(mEnv->NewStringUTF( achi ));
		mEnv->CallStaticBooleanMethod(mActivityClass, mid, jtitle,points);
		mEnv->DeleteLocalRef(jtitle);
	}
}

void Andy_GoogleAnalytics(const char * category, const char * action, const char * label)
{
    log::message("Andy_GoogleAnalytics: %s", action);
    jmethodID mid;
    JNIEnv *mEnv = jniGetEnv();
    jclass mActivityClass = (jclass)mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

    if (mActivityClass)
    {
        log::message("find mActivityClass");
    }

    mid = mEnv->GetStaticMethodID(mActivityClass, "googleAnalyticsFunc", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z");
    if (mid) {
        log::message("find mid");
        jstring jtitle0 = (jstring)(mEnv->NewStringUTF(category));
        jstring jtitle = (jstring)(mEnv->NewStringUTF(action));
        jstring jtitle2 = (jstring)(mEnv->NewStringUTF(label));
        mEnv->CallStaticBooleanMethod(mActivityClass, mid, jtitle0, jtitle, jtitle2);
        mEnv->DeleteLocalRef(jtitle);
        mEnv->DeleteLocalRef(jtitle2);
    }
}


void Andy_SharedFunc( const char * achi, const char * param )
{
	log::message("unlock Andy_SharedFunc: %s", achi);
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "sharedFunc", "(Ljava/lang/String;Ljava/lang/String;)Z");
	if (mid) {
		log::message("find mid");
		jstring jtitle = (jstring)(mEnv->NewStringUTF( achi ));
		jstring jtitle2 = (jstring)(mEnv->NewStringUTF( param ));
		mEnv->CallStaticBooleanMethod(mActivityClass, mid, jtitle, jtitle2);
		mEnv->DeleteLocalRef(jtitle);
		mEnv->DeleteLocalRef(jtitle2);
	}
}

bool Andy_IsGoogle()
{
	log::message("unlock Andy_IsGoogle");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "isGoogle", "()Z");
	if (mid) {
		log::message("find mid");
		return mEnv->CallStaticBooleanMethod(mActivityClass, mid);
	}

	return false;
}

void Andy_Start_App()
{
	log::message("Andy_Start_App start game!!!:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "onStartGame", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid);
	}
}


void Andy_Stop_App()
{
	log::message("Andy_Stop_App:");
	jmethodID mid;
	JNIEnv *mEnv = jniGetEnv();
	jclass mActivityClass = (jclass) mEnv->NewGlobalRef(mEnv->FindClass("com/divol13/magic2048/MainActivity"));

	if ( mActivityClass )
	{
		log::message("find mActivityClass");
	}

	mid= mEnv->GetStaticMethodID(mActivityClass, "onStopGame", "()V");
	if (mid) {
		log::message("find mid");
		mEnv->CallStaticVoidMethod(mActivityClass, mid);
	}
}

